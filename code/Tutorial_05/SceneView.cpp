#include "SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>

#include "Vertex.h"

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const Vertex sg_vertexes[] = {
  // Face 1 (Front)
	VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
	VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
	VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
	VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
	VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
	VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
	VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
	VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
	VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
	VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
	VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
	VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR


SceneView::SceneView() :
	m_needRepaint(false)
{
	// create camera
//	m_camera.translate(0,5,0);
//	m_camera.rotate(-30, m_camera.right());

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
	grid.m_uniformNames.append("worldToView");
	grid.m_uniformNames.append("gridColor");
	grid.m_uniformNames.append("backgroundColor");
	m_shaderPrograms.append( grid );
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

	// initialize drawable objects
//	m_gridObject.create(m_shaderPrograms[1].shaderProgram());

	// tell OpenGL to show only faces whose normal vector points towards us
	glEnable(GL_CULL_FACE);

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

	// Application-specific initialization
	{
		QOpenGLShaderProgram		*program = m_shaderPrograms[0].shaderProgram();

		// Create Buffer (Do not release until VAO is created)
		m_vertexDataBuffer.create();
		m_vertexDataBuffer.bind();
		m_vertexDataBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		int vertexArrayMemSize = sizeof(sg_vertexes);
		m_vertexDataBuffer.allocate(sg_vertexes, vertexArrayMemSize);

		// Create Vertex Array Object
		m_vao.create(); // create underlying OpenGL object
		m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

		// tell shader program we have two data arrays to be used as input to the shaders
		// the two calls to setAttributeBuffer() reference again the buffer whose allocate() function was called last,
		// in this case m_vertexDataBuffer.
		program->enableAttributeArray(0); // array with index/id 0
		program->enableAttributeArray(1); // array with index/id 1
		// index 0 = position
		program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		// index 0 = color
		program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		// Release (unbind) all
		m_vertexDataBuffer.release();
		m_vao.release();
//		m_program->release();
	} // end data init
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
	// only paint if we need to
	if (!m_needRepaint)
		return;

	qDebug() << "[SceneView::paintGL]";

	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	// set the background color = clear color
	QVector3D backgroundColor(0.1f, 0.1f, 0.3f);
	glClearColor(backgroundColor.x(), backgroundColor.y(), backgroundColor.z(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// clear the background color
	// render using our shader
	m_shaderPrograms[0].shaderProgram()->bind();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	qDebug() << m_worldToView;
	m_shaderPrograms[0].shaderProgram()->setUniformValue(u_worldToView, m_worldToView);

	{
		// set the geometry ("position" and "color" arrays)
		m_vao.bind();

		// now draw the cube by drawing individual triangles
		// - GL_TRIANGLES - draw individual triangles
		int vertexCount = sizeof(sg_vertexes)/sizeof(sg_vertexes[0]);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		// release vertices again
		m_vao.release();
	}
	m_shaderPrograms[0].shaderProgram()->release();

#ifdef DRAW_GRID
	QOpenGLShaderProgram * gridProgram = m_shaderPrograms[1].shaderProgram();
	if (!gridProgram->bind())
		qDebug() << gridProgram->log();

	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	gridProgram->setUniformValue(m_shaderPrograms[1].m_uniformIDs[0], m_worldToView);
	QVector3D gridColor(0.3f, 0.6f, 0.3f);
	gridProgram->setUniformValue(m_shaderPrograms[1].m_uniformIDs[1], gridColor);
	gridProgram->setUniformValue(m_shaderPrograms[1].m_uniformIDs[2], backgroundColor);
	{
		// bind vertex array object
		m_gridObject.m_vao.bind();

		// now draw the grid lines
		glDrawArrays(GL_LINES, 0, m_gridObject.m_NVertexes);
		// release vertices again
		m_gridObject.m_vao.release();
	}
	gridProgram->release();
	m_needRepaint = false;
#endif
}


void SceneView::exposeEvent(QExposeEvent *event) {
	if (event->region() != m_cachedRegion) {
		// the check against the cached region is needed for Windows OS.
		// There, when the window is moved around, the function onFrameSwapped() is called
		// due to the frameSwapped() signal and also exposeEvent() is sent, because
		// the window manager tells us that the entire screen is invalidated.
		// This results in two QOpenGLWindow::update() calls and a noticable 2 vsync delay.
		m_cachedRegion = event->region();
		qDebug() << "SceneView::exposeEvent" << m_cachedRegion;

		m_needRepaint = true;
		OpenGLWindow::exposeEvent(event); // this will trigger a repaint
	}
	else {
		event->ignore();
	}
}


void SceneView::updateWorld2ViewMatrix() {
	qDebug() << "SceneView::updateWorld2ViewMatrix";
	// projectedPoint = camera2project * world2camera
	Transform3D transform;
	transform.translate(0.0f, 0.0f, -5.0f);

	m_worldToView = m_projection * m_camera.toMatrix() * transform.toMatrix();
	qDebug() << "m_projection = " << m_projection;
	qDebug() << "m_camera = " << m_camera.toMatrix();
	qDebug() << "m_transform = " << transform.toMatrix();
	qDebug() << "world2View = " << m_worldToView;
	m_needRepaint = true;
}
