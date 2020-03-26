#include "OpenGLRenderWindow.h"

#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QOpenGLShaderProgram>
#include <QExposeEvent>

#include "Vertex.h"

#define SHADER(x) m_shaderPrograms[x].shaderProgram()

OpenGLRenderWindow::OpenGLRenderWindow() :
	m_animate(true),
	m_sceneChanged(false),
	m_elementBuffer(QOpenGLBuffer::IndexBuffer)
{
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

	// move object a little bit to the back of the scene (negative z coordinates = further back)
	m_transform.translate(0.0f, 0.0f, -5.0f);
	m_camera.translate(0,5,0);
	m_camera.rotate(-30, m_camera.right());
	m_updateNeeded = true;

	m_inputHandler.addRecognizedKey(Qt::Key_W);
	m_inputHandler.addRecognizedKey(Qt::Key_A);
	m_inputHandler.addRecognizedKey(Qt::Key_S);
	m_inputHandler.addRecognizedKey(Qt::Key_D);
	m_inputHandler.addRecognizedKey(Qt::Key_Q);
	m_inputHandler.addRecognizedKey(Qt::Key_E);

	// Front Verticies
	#define VERTEX_FTR QVector3D( 0.5f,  0.5f,  0.5f)
	#define VERTEX_FTL QVector3D(-0.5f,  0.5f,  0.5f)
	#define VERTEX_FBL QVector3D(-0.5f, -0.0f,  0.5f)
	#define VERTEX_FBR QVector3D( 0.5f, -0.0f,  0.5f)

	// Back Verticies
	#define VERTEX_BTR QVector3D( 0.5f,  0.5f, -0.5f)
	#define VERTEX_BTL QVector3D(-0.5f,  0.5f, -0.5f)
	#define VERTEX_BBL QVector3D(-0.5f, -0.0f, -0.5f)
	#define VERTEX_BBR QVector3D( 0.5f, -0.0f, -0.5f)

	// front
	m_rectangles.push_back( RectMesh(VERTEX_FBL, VERTEX_FBR, VERTEX_FTL, Qt::red ));
	// right
	m_rectangles.push_back( RectMesh(VERTEX_FBR, VERTEX_BBR, VERTEX_FTR, Qt::green ));
	// back
	m_rectangles.push_back( RectMesh(VERTEX_BBR, VERTEX_BBL, VERTEX_BTR, Qt::blue ));
	// left
	m_rectangles.push_back( RectMesh(VERTEX_BBL, VERTEX_FBL, VERTEX_BTL, Qt::yellow ));
	// top
	m_rectangles.push_back( RectMesh(VERTEX_FTL, VERTEX_FTR, VERTEX_BTL, Qt::cyan ));
	// bottom
	m_rectangles.push_back( RectMesh(VERTEX_BBL, VERTEX_BBR, VERTEX_FBL, Qt::magenta ));

	// bottom 2
//	m_rectangles.push_back( RectMesh(VERTEX_BBL, VERTEX_BBR, VERTEX_FBL, Qt::magenta ));
//	m_rectangles.back().m_a.setZ(5);
//	m_rectangles.back().m_b.setZ(5);
//	m_rectangles.back().m_c.setZ(5);

	#undef VERTEX_BBR
	#undef VERTEX_BBL
	#undef VERTEX_BTL
	#undef VERTEX_BTR

	#undef VERTEX_FBR
	#undef VERTEX_FBL
	#undef VERTEX_FTL
	#undef VERTEX_FTR

	// face count
	unsigned int Nrects = m_rectangles.size();

	// we have 6 sides of a cube, and each side needs 4 vertexes, and each vertex requires 2 vectors3d of float
	unsigned int sizeOfVertex = 2*3; // number of floats
	m_boxObject.m_vertexBufferData.resize(Nrects*4*sizeOfVertex);
	std::fill(m_boxObject.m_vertexBufferData.begin(), m_boxObject.m_vertexBufferData.end(), 0);

	// we have 6 sides of cube, and each side has two triangles, with 3 indexes each
	m_boxObject.m_elementBufferData.resize(Nrects*2*3);
	std::fill(m_boxObject.m_elementBufferData.begin(), m_boxObject.m_elementBufferData.end(), 0);
	updateScene();
}



OpenGLRenderWindow::~OpenGLRenderWindow() {
	// resource cleanup
	m_vao.destroy();
	m_boxObject.m_vertexBuffer.destroy();
	for (ShaderProgram & p : m_shaderPrograms)
		p.destroy();
}


void OpenGLRenderWindow::initializeGL() {
	// this function is called once, when the window is first initialized

	// Initialize OpenGL Backend
	initializeOpenGLFunctions(); // QOpenGLFunctions::initializeOpenGLFunctions()

	// we process all individual shader programs first
	for (ShaderProgram & p : m_shaderPrograms)
		p.create();

	// The 'frameSwapped' signal is emitted after the widget's top-level window has finished
	// composition and returned from its potentially blocking QOpenGLContext::swapBuffers() call.
	// We then call update and repaint the scene.
	connect(this, SIGNAL(frameSwapped()), this, SLOT(onFrameSwapped()));

	printVersionInformation();

	// tell OpenGL to show only faces whose normal vector points towards us
	glEnable(GL_CULL_FACE);

	// Enable depth testing, important for the grid and for the drawing order of several objects
	glEnable(GL_DEPTH_TEST);  // Enables Depth Testing
	glDepthFunc(GL_LESS);     // The Type Of Depth Test To Do


	// Application-specific initialization
	{
		// Create Shader (Do not release until VAO is created)

		// Create Vertex Array Object
		m_vao.create(); // create Vertex Array Object

		m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

		// Create Buffer (Do not release until VAO is created and released)
		m_boxObject.m_vertexBuffer.create();
		m_boxObject.m_vertexBuffer.bind();
		m_boxObject.m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		int vertexMemSize = m_boxObject.m_vertexBufferData.size()*sizeof(float);
		m_boxObject.m_vertexBuffer.allocate(m_boxObject.m_vertexBufferData.data(), vertexMemSize);

		m_elementBuffer.create();
		m_elementBuffer.bind();
		m_elementBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		int elementMemSize = m_boxObject.m_elementBufferData.size()*sizeof(GLuint);
		m_elementBuffer.allocate(m_boxObject.m_elementBufferData.data(), elementMemSize);

		// tell shader program we have two data arrays to be used as input to the shaders
		// the two calls to setAttributeBuffer() reference again the 'vertex' buffer whose allocate() function was called last,
		// in this case m_vertexDataBuffer.

		// index 0 = position
		SHADER(0)->enableAttributeArray(0); // array with index/id 0
		SHADER(0)->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		// index 1 = color
		SHADER(0)->enableAttributeArray(1); // array with index/id 1
		SHADER(0)->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		m_vao.release();
		// Release (unbind) all
		m_boxObject.m_vertexBuffer.release();
		m_elementBuffer.release();
	} // end data init

	m_gridObject.create(SHADER(1));
}


void OpenGLRenderWindow::resizeGL(int width, int height) {
	// the projection matrix need to be updated only for window size changes
	m_projection.setToIdentity();
	// create projection matrix, i.e. camera lens
	// BUG : Mind, to not use 0.0 for near plane!
	m_projection.perspective(45.0f, width / float(height), 0.1f, 1000.0f);
	m_updateNeeded = true;
}


void OpenGLRenderWindow::paintGL() {
	// this function is called for every frame to be rendered on screen

	// monitor rendering - prints out a string whenever the rendering takes place - only for resizing or when
	// focussing in/out of the window
//	qDebug() << QDateTime::currentDateTime().toString();

	// clear the background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the background color = clear color
	QVector3D backColor(0.1f, 0.15f, 0.3f);
	glClearColor(0.1f, 0.15f, 0.3f, 1.0f);

	// render using our shader
	SHADER(0)->bind();

	QMatrix4x4 worldToView = m_projection * m_camera.toMatrix() * m_transform.toMatrix();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[0], worldToView);

	{
		// set the geometry ("position" and "color" arrays)
		m_vao.bind();

		// now draw the cube by drawing individual triangles
		// - GL_TRIANGLES - draw individual triangles via elements
		glDrawElements(GL_TRIANGLES, m_boxObject.m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
		// release vertices again
		m_vao.release();
	}
	SHADER(0)->release();

	// *** render grid afterwards ***

	// render using our shader
	SHADER(1)->bind();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[0], worldToView);
	QVector3D color(0.3f, 0.3f, 0.6f);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[1], color);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[2], backColor);

	{
		// set the geometry ("position" and "color" arrays)
		m_gridObject.m_vao.bind();

		// now draw the grid lines
		glDrawArrays(GL_LINES, 0, m_gridObject.m_NVertexes);
		// release vertices again
		m_gridObject.m_vao.release();
	}
	SHADER(1)->release();


}


void OpenGLRenderWindow::onFrameSwapped() {
	// this is called approximately in sync with the monitors framerate

	// if the right mouse button is held down, we check for state changes
	if (m_inputHandler.buttonDown(Qt::RightButton)) {
		QPoint currentPos = QCursor::pos();
		bool mouseMoved = m_inputHandler.mouseDownPos() != currentPos;

		// Handle translations
		QVector3D translation;
		bool keyPressed = false;
		if (m_inputHandler.keyDown(Qt::Key_W)) {
			translation += m_camera.forward();
			keyPressed = true;
		}
		if (m_inputHandler.keyDown(Qt::Key_S)) {
			translation -= m_camera.forward();
			keyPressed = true;
		}
		if (m_inputHandler.keyDown(Qt::Key_A)) {
			translation -= m_camera.right();
			keyPressed = true;
		}
		if (m_inputHandler.keyDown(Qt::Key_D)) {
			translation += m_camera.right();
			keyPressed = true;
		}
		if (m_inputHandler.keyDown(Qt::Key_Q)) {
			translation -= m_camera.up();
			keyPressed = true;
		}
		if (m_inputHandler.keyDown(Qt::Key_E)) {
			translation += m_camera.up();
			keyPressed = true;
		}

		if (mouseMoved || keyPressed) {
			qDebug() << "Input has changed: mouseMoved " << (mouseMoved ? "yes" : "no") << ", keys changed " << (keyPressed ? "yes" : "no");

			static const float transSpeed = 0.1f;
			static const float rotSpeed   = 0.1f;

			// Handle rotations
			if (mouseMoved) {
				QPoint mouseDelta = m_inputHandler.mouseDelta(currentPos); // resets the internal position
				m_camera.rotate(-rotSpeed * mouseDelta.x(), Camera::LocalUp);
				m_camera.rotate(-rotSpeed * mouseDelta.y(), m_camera.right());
			}

			if (keyPressed)
				m_camera.translate(transSpeed * translation);

			m_updateNeeded = true;
		}
	}

	// when doing animation, updating the GLWindow here might be useful,
	// for rather static drawing, we should really only update
	// the GL scene, when something changes

	if (m_sceneChanged) {
		updateScene();
		m_updateNeeded = true;
	}

	//	qDebug() << "OpenGLRenderWindow::onFrameSwapped";
	if (m_animate) {
		// Update instance information
		m_transform.rotate(1.0f, QVector3D(0.0f, 0.1f, 0.0f));

		m_updateNeeded = true;
	}

	if (m_updateNeeded) {
		// Schedule a redraw
		QOpenGLWindow::update();
		m_updateNeeded = false;
	}

}

void OpenGLRenderWindow::keyPressEvent(QKeyEvent *event) {
	if (event->isAutoRepeat()) {
		event->ignore();
	}
	else {
		if (m_inputHandler.keyPressed(static_cast<Qt::Key>((event->key()))))
			if (m_inputHandler.buttonDown(Qt::RightButton))
				onFrameSwapped();
	}
}


void OpenGLRenderWindow::keyReleaseEvent(QKeyEvent *event) {
	if (event->isAutoRepeat())	{
		event->ignore();
	}
	else {
		if (m_inputHandler.keyReleased(static_cast<Qt::Key>((event->key()))))
			if (m_inputHandler.buttonDown(Qt::RightButton))
				onFrameSwapped();
	}
}


void OpenGLRenderWindow::mousePressEvent(QMouseEvent *event) {
	m_inputHandler.buttonPressed(static_cast<Qt::MouseButton>(event->button()), QCursor::pos());
	if (m_inputHandler.buttonDown(Qt::RightButton))
		onFrameSwapped();
}


void OpenGLRenderWindow::mouseReleaseEvent(QMouseEvent *event) {
	m_inputHandler.buttonReleased(static_cast<Qt::MouseButton>(event->button()));
	if (m_inputHandler.buttonDown(Qt::RightButton))
		onFrameSwapped();
}


void OpenGLRenderWindow::mouseMoveEvent(QMouseEvent *) {
	if (m_inputHandler.buttonDown(Qt::RightButton))
		onFrameSwapped();
}



void OpenGLRenderWindow::exposeEvent(QExposeEvent *ev) {
	if (ev->region() != m_cachedRegion) {
		// the check against the cached region is needed for Windows OS.
		// There, when the window is moved around, the function onFrameSwapped() is called
		// due to the frameSwapped() signal and also exposeEvent() is sent, because
		// the window manager tells us that the entire screen is invalidated.
		// This results in two QOpenGLWindow::update() calls and a noticable 2 vsync delay.
		m_cachedRegion = ev->region();
		qDebug() << "OpenGLRenderWindow::exposeEvent" << m_cachedRegion;

		m_updateNeeded = true;
		QOpenGLWindow::exposeEvent(ev);
	}
	else {
		ev->ignore();
	}
}


void OpenGLRenderWindow::updateScene() {
	// update the buffers
	float * vertexBuffer = m_boxObject.m_vertexBufferData.data();
	unsigned int vertexCount = 0;
	GLuint * elementBuffer = m_boxObject.m_elementBufferData.data();
	unsigned int elementCount = 0;
	for (unsigned int i=0; i<m_rectangles.size(); ++i) {
		m_rectangles[i].copy2Buffer(vertexBuffer, vertexCount, elementBuffer, elementCount);
		vertexCount += 4;
		elementCount += 2;
	}
	m_boxObject.m_vertexBuffer.bind();
	m_boxObject.m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_boxObject.m_vertexBufferData.size()*sizeof(float);
	m_boxObject.m_vertexBuffer.allocate(m_boxObject.m_vertexBufferData.data(), vertexMemSize);
	m_sceneChanged = false;
}


void OpenGLRenderWindow::printVersionInformation() {
	QString glType;
	QString glVersion;
	QString glProfile;

	// Get Version Information
	glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	// Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break

	switch (format().profile()) {
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE

	// qPrintable() will print our QString w/o quotes around it.
//	qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
	// alternatively:
//	qDebug().noquote() << glType << glVersion << "(" << glProfile << ")";
	qDebug().noquote().nospace() << glType << " " << glVersion << " (" << glProfile << ")";
}
