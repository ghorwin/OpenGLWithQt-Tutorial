/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>
#include <QDateTime>

#include "DebugApplication.h"

#define SHADER(x) m_shaderPrograms[x].shaderProgram()

const QVector3D UP_VECTOR = QVector3D(0.0f, 1.0f, 0.0f);
const unsigned int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;

QVector3D LIGHT_POS(500.0f, 1000.0f, -750.0f);

SceneView::SceneView() :
	m_inputEventReceived(false),
	m_frameBufferObject(nullptr)
{
	// tell keyboard handler to monitor certain keys
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_W);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_A);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_S);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_D);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Q);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_E);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Shift);

	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
	ShaderProgram blocks(":/shaders/sceneWithShadowMap.vert",":/shaders/sceneWithShadowMap.frag");
	blocks.m_uniformNames.append("worldToView");         // #0
	blocks.m_uniformNames.append("lightSpaceMatrix");    // #1
	blocks.m_uniformNames.append("lightPos");            // #2
	blocks.m_uniformNames.append("viewPos");             // #3
	blocks.m_uniformNames.append("shadowMap");           // #4
	m_shaderPrograms.append( blocks );

	// Shaderprogram #1 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/grid.frag");
	grid.m_uniformNames.append("worldToView"); // mat4
	grid.m_uniformNames.append("gridColor"); // vec3
	grid.m_uniformNames.append("backColor"); // vec3
	m_shaderPrograms.append( grid );

	// Shaderprogram #2 : only for shadow/depth map
	ShaderProgram shadow(":/shaders/depthMap.vert",":/shaders/depthMap.frag");
	shadow.m_uniformNames.append("worldToView");
	m_shaderPrograms.append( shadow );

	// Shaderprogram #3 : screenfill (for debugging, only)
	ShaderProgram screenFill(":/shaders/screenfill.vert",":/shaders/screenfill.frag");
	m_shaderPrograms.append( screenFill );

	// *** initialize camera placement and model placement in the world

	// move camera a little back and up
	m_camera.translate(50,100,-150);
	// look slightly down
	m_camera.rotate(-30, m_camera.right());
	// look slightly right
	m_camera.rotate(155, UP_VECTOR);
}


SceneView::~SceneView() {
	if (m_context) {
		m_context->makeCurrent(this);

		for (ShaderProgram & p : m_shaderPrograms)
			p.destroy();

		m_boxObject.destroy();
		m_gridObject.destroy();
		m_texture2ScreenObject.destroy();

		m_gpuTimers.destroy();

		delete m_frameBufferObject;
	}
}


void SceneView::initializeGL() {
	FUNCID(SceneView::initializeGL);
	try {
		// initialize shader programs
		for (ShaderProgram & p : m_shaderPrograms)
			p.create();

		// tell OpenGL to show only faces whose normal vector points towards us
		glEnable(GL_CULL_FACE);
		// enable depth testing
		glEnable(GL_DEPTH_TEST);
		// set the background color = clear color
		glClearColor(0.1f, 0.15f, 0.3f, 1.0f);

		// initialize drawable objects
		m_boxObject.create(SHADER(0));
		m_gridObject.create(SHADER(1));
		m_texture2ScreenObject.create(SHADER(3));

		// Timer
		m_gpuTimers.setSampleCount(7);
		m_gpuTimers.create();

		// generate framebuffer for depth map
		glGenFramebuffers(1, &depthMapFBO);

		// generate depth map texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		// create the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		// and set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach depth texture to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

		// explicitely tell OpenGL that we do not want to render to color buffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			qDebug() << "Framebuffer complete";
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind framebuffer

		QMatrix4x4 lightProjection;
		float near_plane = 1.0f;
		float far_plane = 10000.5f;
		lightProjection.ortho(-100.f, 100.f, -100.f, 100.f, near_plane, far_plane);
		QMatrix4x4 lightCam;
		lightCam.setToIdentity();
		lightCam.lookAt( LIGHT_POS,
						 QVector3D(0,0,0),
						 UP_VECTOR);

		m_lightSpaceMatrix = lightProjection * lightCam * m_transform.toMatrix();

		SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[4], 0); // uniform #4 = "shadowMap" -> bind to TEXTURE0
	}
	catch (OpenGLException & ex) {
		throw OpenGLException(ex, "OpenGL initialization failed.", FUNC_ID);
	}
}


void SceneView::resizeGL(int width, int height) {
	// the projection matrix need to be updated only for window size changes
	m_projection.setToIdentity();
	// create projection matrix, i.e. camera lens
	m_projection.perspective(
				/* vertical angle */ 45.0f,
				/* aspect ratio */   width / float(height),
				/* near */           0.1f,
				/* far */            1000.0f
		);
	// Mind: to not use 0.0 for near plane, otherwise depth buffering and depth testing won't work!

	// update cached world2view matrix
	updateWorld2ViewMatrix();
}


void SceneView::paintGL() {
	m_cpuTimer.start();
	if (((DebugApplication *)qApp)->m_aboutToTerminate)
		return;

	// process input, i.e. check if any keys have been pressed
	if (m_inputEventReceived)
		processInput();

	m_gpuTimers.reset();

	m_gpuTimers.recordSample(); // render shadow map

	// *** render shadow map ***
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		SHADER(2)->bind();
		SHADER(2)->setUniformValue(m_shaderPrograms[2].m_uniformIDs[0], m_lightSpaceMatrix);

		m_gpuTimers.recordSample(); // render main scene
		m_boxObject.render();
		SHADER(2)->release();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_gpuTimers.recordSample(); // render main scene

	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	qDebug() << "SceneView::paintGL(): Rendering to:" << width()* retinaScale << "x" << height()* retinaScale;

	// set the background color = clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind depthmap to TEXTURE0 -> maps to "shadowMap" texture in shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

//#define RENDER_DEPTHMAP
#ifdef RENDER_DEPTHMAP
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	//	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	SHADER(3)->bind();

//	m_gpuTimers.recordSample(); // render framebuffer
	m_texture2ScreenObject.render();
	glEnable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

#else
	// *** render boxes ***

	SHADER(0)->bind();
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[0], m_worldToView);
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[1], m_lightSpaceMatrix);
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[2], LIGHT_POS); // lightPos
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[3], m_camera.translation()); // cameraPos
	m_gpuTimers.recordSample(); // render main scene

	m_boxObject.render();
	SHADER(0)->release();


	// *** render grid ***

	QVector3D backColor(0.1f, 0.15f, 0.3f);
	QVector3D gridColor(0.5f, 0.5f, 0.7f);

	m_gpuTimers.recordSample(); // render grid
	SHADER(1)->bind();
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[0], m_worldToView);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[1], gridColor);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[2], backColor);

	m_gpuTimers.recordSample(); // render main scene
	m_gridObject.render();
	SHADER(1)->release();

#endif // RENDER_DEPTHMAP

#if 0
	// do some animation stuff
	m_transform.rotate(1.0f, QVector3D(0.0f, 0.1f, 0.0f));
	updateWorld2ViewMatrix();
	renderLater();
#endif

	m_gpuTimers.recordSample(); // done painting

	checkInput();

	QVector<GLuint64> intervals = m_gpuTimers.waitForIntervals();
	for (GLuint64 it : intervals)
		qDebug() << "  " << it*1e-6 << "ms/frame";
	QVector<GLuint64> samples = m_gpuTimers.waitForSamples();
	qDebug() << "Total render time: " << (samples.back() - samples.front())*1e-6 << "ms/frame";

	qint64 elapsedMs = m_cpuTimer.elapsed();
	qDebug() << "Total paintGL time: " << elapsedMs << "ms";
}


void SceneView::keyPressEvent(QKeyEvent *event) {
	m_keyboardMouseHandler.keyPressEvent(event);
	checkInput();
}

void SceneView::keyReleaseEvent(QKeyEvent *event) {
	m_keyboardMouseHandler.keyReleaseEvent(event);
	checkInput();
}

void SceneView::mousePressEvent(QMouseEvent *event) {
	m_keyboardMouseHandler.mousePressEvent(event);
	checkInput();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
	m_keyboardMouseHandler.mouseReleaseEvent(event);
	checkInput();
}

void SceneView::mouseMoveEvent(QMouseEvent * /*event*/) {
	checkInput();
}

void SceneView::wheelEvent(QWheelEvent *event) {
	m_keyboardMouseHandler.wheelEvent(event);
	checkInput();
}


void SceneView::checkInput() {
	// this function is called whenever _any_ key/mouse event was issued

	// we test, if the current state of the key handler requires a scene update
	// (camera movement) and if so, we just set a flag to do that upon next repaint
	// and we schedule a repaint

	// trigger key held?
	if (m_keyboardMouseHandler.buttonDown(Qt::RightButton)) {
		// any of the interesting keys held?
		if (m_keyboardMouseHandler.keyDown(Qt::Key_W) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_A) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_S) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_D) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_Q) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_E))
		{
			m_inputEventReceived = true;
//			qDebug() << "SceneView::checkInput() inputEventReceived";
			renderLater();
			return;
		}

		// has the mouse been moved?
		if (m_keyboardMouseHandler.mouseDownPos() != QCursor::pos()) {
			m_inputEventReceived = true;
//			qDebug() << "SceneView::checkInput() inputEventReceived: " << QCursor::pos() << m_keyboardMouseHandler.mouseDownPos();
			renderLater();
			return;
		}
	}
	// scroll-wheel turned?
	if (m_keyboardMouseHandler.wheelDelta() != 0) {
		m_inputEventReceived = true;
		renderLater();
		return;
	}
}


void SceneView::processInput() {
	// function must only be called if an input event has been received
	Q_ASSERT(m_inputEventReceived);
	m_inputEventReceived = false;
//	qDebug() << "SceneView::processInput()";

	// check for trigger key
	if (m_keyboardMouseHandler.buttonDown(Qt::RightButton)) {

		// Handle translations
		QVector3D translation;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_W)) 		translation += m_camera.forward();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_S)) 		translation -= m_camera.forward();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_A)) 		translation -= m_camera.right();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_D)) 		translation += m_camera.right();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Q)) 		translation -= m_camera.up();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_E)) 		translation += m_camera.up();

		float transSpeed = 0.8f;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Shift))
			transSpeed = 0.1f;
		m_camera.translate(transSpeed * translation);

		// Handle rotations
		// get and reset mouse delta (pass current mouse cursor position)
		QPoint mouseDelta = m_keyboardMouseHandler.resetMouseDelta(QCursor::pos()); // resets the internal position
		static const float rotatationSpeed  = 0.4f;
		const QVector3D LocalUp(0.0f, 1.0f, 0.0f); // same as in Camera::up()
		m_camera.rotate(-rotatationSpeed * mouseDelta.x(), LocalUp);
		m_camera.rotate(-rotatationSpeed * mouseDelta.y(), m_camera.right());

		// finally, reset "WasPressed" key states
		m_keyboardMouseHandler.clearWasPressedKeyStates();
	}
	int wheelDelta = m_keyboardMouseHandler.resetWheelDelta();
	if (wheelDelta != 0) {
		float transSpeed = 8.f;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Shift))
			transSpeed = 0.8f;
		m_camera.translate(wheelDelta * transSpeed * m_camera.forward());
	}

	updateWorld2ViewMatrix();
	// not need to request update here, since we are called from paint anyway
}


void SceneView::updateWorld2ViewMatrix() {
	// transformation steps:
	//   model space -> transform -> world space
	//   world space -> camera/eye -> camera view
	//   camera view -> projection -> normalized device coordinates (NDC)
	m_worldToView = m_projection * m_camera.toMatrix() * m_transform.toMatrix();
}


