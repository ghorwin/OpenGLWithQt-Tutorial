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

#define SHADER(x) m_shaderPrograms[x].shaderProgram()

SceneView::SceneView() :
	m_inputEventReceived(false)
{

	// tell keyboard handler to monitor certain keys
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_W);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_A);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_S);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_D);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Q);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_E);

	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
	ShaderProgram blocks(":/shaders/withWorldAndCamera.vert",":/shaders/simple.frag");
	blocks.m_uniformNames.append("worldToView");
	m_shaderPrograms.append( blocks );

	// Shaderprogram #1 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/simple.frag");
	grid.m_uniformNames.append("worldToView"); // mat4
	grid.m_uniformNames.append("gridColor"); // vec3
	grid.m_uniformNames.append("backColor"); // vec3
	m_shaderPrograms.append( grid );

	// move objects a little bit to the back of the scene (negative z coordinates = further back)
	m_transform.translate(0.0f, 0.0f, -5.0f);
	m_camera.translate(0,5,0);
	m_camera.rotate(-30, m_camera.right());
}


SceneView::~SceneView() {
	m_context->makeCurrent(this);

	for (ShaderProgram & p : m_shaderPrograms)
		p.destroy();

}


void SceneView::initializeGL() {

	// we process all individual shader programs first
	for (ShaderProgram & p : m_shaderPrograms)
		p.create();

	// tell OpenGL to show only faces whose normal vector points towards us
	glEnable(GL_CULL_FACE);

	// Enable depth testing, important for the grid and for the drawing order of several objects
	glEnable(GL_DEPTH_TEST);  // Enables Depth Testing
	glDepthFunc(GL_LESS);     // The Type Of Depth Test To Do

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

	// initialize drawable objects
	m_boxObject.create(SHADER(0));
	m_gridObject.create(SHADER(1));
}


void SceneView::resizeGL(int width, int height) {
	// the projection matrix need to be updated only for window size changes
	m_projection.setToIdentity();
	// create projection matrix, i.e. camera lens
	m_projection.perspective(
				/* vertical angle */ 45.0f,
				/* aspect ratio */   width / float(height),
				/* near */           0.1f,
				/* fa r*/            1000.0f
		);
	// Mind: to not use 0.0 for near plane, otherwise depth buffering and depth testing won't work!

	updateWorld2ViewMatrix();
}


void SceneView::paintGL() {
//	qDebug() << "SceneView::paintGL()";
	// process input, i.e. check if any keys have been pressed
	if (m_inputEventReceived)
		processInput(); // this sets the m_needRepaint flag if user has pressed some keys

//	qDebug() << "SceneView::paintGL() actual painting";
	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	// set the background color = clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the background color = clear color
	QVector3D backColor(0.1f, 0.15f, 0.3f);
	glClearColor(0.1f, 0.15f, 0.3f, 1.0f);

	QVector3D gridColor(0.3f, 0.3f, 0.6f);

	// *** render box
	SHADER(0)->bind();
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[0], m_worldToView);
	m_boxObject.render(); // render the box
	SHADER(0)->release();

	// *** render grid afterwards ***

	SHADER(1)->bind();
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[0], m_worldToView);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[1], gridColor);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[2], backColor);
	m_gridObject.render(); // render the grid
	SHADER(1)->release();

#if 0
	// do some animation stuff
	m_transform.rotate(1.0f, QVector3D(0.0f, 0.1f, 0.0f));
	updateWorld2ViewMatrix();
	renderLater();
#endif

	checkInput();
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

void SceneView::mouseMoveEvent(QMouseEvent *event) {
	checkInput();
}

void SceneView::wheelEvent(QWheelEvent *event) {
	m_keyboardMouseHandler.wheelEvent(event);
	checkInput();
}


void SceneView::exposeEvent(QExposeEvent *event) {
	if (event->region() != m_cachedRegion) {
		// the check against the cached region is needed for Windows OS.
		// There, when the window is moved around, the function onFrameSwapped() is called
		// due to the frameSwapped() signal and also exposeEvent() is sent, because
		// the window manager tells us that the entire screen is invalidated.
		// This results in two QOpenGLWindow::update() calls and a noticable 2 vsync delay.
		m_cachedRegion = event->region();
		//qDebug() << "SceneView::exposeEvent" << m_cachedRegion;

		OpenGLWindow::exposeEvent(event); // this will trigger a repaint
	}
	else {
		event->ignore();
	}
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
}


void SceneView::processInput() {
	// function must only be called if an input event has been received
	Q_ASSERT(m_inputEventReceived);
	m_inputEventReceived = false;
//	qDebug() << "SceneView::processInput()";

	QPoint currentPos = QCursor::pos();

	// Handle translations
	QVector3D translation;

	if (m_keyboardMouseHandler.keyDown(Qt::Key_W)) 		translation += m_camera.forward();
	if (m_keyboardMouseHandler.keyDown(Qt::Key_S)) 		translation -= m_camera.forward();
	if (m_keyboardMouseHandler.keyDown(Qt::Key_A)) 		translation -= m_camera.right();
	if (m_keyboardMouseHandler.keyDown(Qt::Key_D)) 		translation += m_camera.right();
	if (m_keyboardMouseHandler.keyDown(Qt::Key_Q)) 		translation -= m_camera.up();
	if (m_keyboardMouseHandler.keyDown(Qt::Key_E)) 		translation += m_camera.up();

	// get and reset time delta
	double timeSinceLastCheck = 100;//m_keyboardMouseHandler.timeDelta(); // in ms

	static const float transSpeed = 0.001f;
	static const float rotatationSpeed   = 0.001f;

	// Handle rotations
	// get and reset mouse delta (pass current mouse cursor position)
	QPoint mouseDelta = m_keyboardMouseHandler.mouseDelta(currentPos); // resets the internal position
	const QVector3D LocalUp(0.0f, 1.0f, 0.0f); // same as in Camera::up()
	m_camera.rotate(-rotatationSpeed * timeSinceLastCheck * mouseDelta.x(), LocalUp);
	m_camera.rotate(-rotatationSpeed * timeSinceLastCheck * mouseDelta.y(), m_camera.right());

	m_camera.translate(transSpeed * timeSinceLastCheck * translation);

	// finally, reset "WasPressed" key states
	m_keyboardMouseHandler.clearWasPressedKeyStates();
	updateWorld2ViewMatrix();
}



void SceneView::updateWorld2ViewMatrix() {
	// transformation steps:
	//   model space -> transform -> world space
	//   world space -> camera/eye -> camera view
	//   camera view -> projection -> normalized device coordinates (NDC)
	m_worldToView = m_projection * m_camera.toMatrix() * m_transform.toMatrix();
	renderLater(); // request update
}
