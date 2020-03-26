#include "SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>
#include <QDateTime>

#include "Vertex.h"


#define SHADER(x) m_shaderPrograms[x].shaderProgram()

SceneView::SceneView() :
	m_needRepaint(false)
{
	// create camera
	m_camera.translate(0,2,0);
	m_camera.rotate(-30, m_camera.right());

	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_W);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_A);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_S);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_D);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Q);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_E);

	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
	ShaderProgram blocks(":/shaders/world2view.vert",":/shaders/simple.frag");
	blocks.m_uniformNames.append("worldToView");
	m_shaderPrograms.append( blocks );

	// Shaderprogram #1 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/simple.frag");
	grid.m_uniformNames.append("worldToView");
	grid.m_uniformNames.append("gridColor");
	grid.m_uniformNames.append("backgroundColor");
	m_shaderPrograms.append( grid );


	// old data

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
	m_vertexBufferData.resize(Nrects*4*sizeOfVertex);
	std::fill(m_vertexBufferData.begin(), m_vertexBufferData.end(), 0);

	// we have 6 sides of cube, and each side has two triangles, with 3 indexes each
	m_elementBufferData.resize(Nrects*2*3);
	std::fill(m_elementBufferData.begin(), m_elementBufferData.end(), 0);

	// create the grid lines

	const unsigned int N = 100; // number of lines to draw in x and z direction
	float width = 500;

	// we have 2*N lines, each line requires two vertexes, with two floats (x and z coordinates) each.
	m_gridVertexBufferData.resize(2*N*2*2);
	float * gridVertexBufferPtr = m_gridVertexBufferData.data();
	// compute grid lines with z = const
	float x1 = -width*0.5;
	float x2 = width*0.5;
	for (unsigned int i=0; i<N; ++i, ++gridVertexBufferPtr) {
		float z = width/(N-1)*i-width*0.5;
		*gridVertexBufferPtr = x1;
		*(++gridVertexBufferPtr) = z;
		*(++gridVertexBufferPtr) = x2;
		*(++gridVertexBufferPtr) = z;
	}
	// compute grid lines with x = const
	float z1 = -width*0.5;
	float z2 = width*0.5;
	for (unsigned int i=0; i<N; ++i, ++gridVertexBufferPtr) {
		float x = width/(N-1)*i-width*0.5;
		*gridVertexBufferPtr = x;
		*(++gridVertexBufferPtr) = z1;
		*(++gridVertexBufferPtr) = x;
		*(++gridVertexBufferPtr) = z2;
	}


}


SceneView::~SceneView() {
	m_context->makeCurrent(this);

	for (ShaderProgram & p : m_shaderPrograms)
		p.destroy();
}


void SceneView::initializeGL() {

	// we process all individual shader programs first
//	for (ShaderProgram & p : m_shaderPrograms)
//		p.create();

	// initialize drawable objects
//	m_boxObject.create(SHADER(0));
//	m_gridObject.create(SHADER(1));

	// Application-specific initialization
	{
		// Create Shader (Do not release until VAO is created)

		// This is the same stuff as in Var_01, but shortened a bit
		m_program = new QOpenGLShaderProgram();
		m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/world2view.vert");
		m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
		m_program->link();
		m_program->bind();

		// Cache Uniform Locations
		u_worldToView = m_program->uniformLocation("worldToView");

		// Create Vertex Array Object
		m_vao.create(); // create Vertex Array Object

		m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

		// Create Buffer (Do not release until VAO is created and released)
		m_vertexBuffer.create();
		m_vertexBuffer.bind();
		m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		int vertexMemSize = m_vertexBufferData.size()*sizeof(float);
		m_vertexBuffer.allocate(m_vertexBufferData.data(), vertexMemSize);

		m_elementBuffer.create();
		m_elementBuffer.bind();
		m_elementBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
		m_elementBuffer.allocate(m_elementBufferData.data(), elementMemSize);

		// tell shader program we have two data arrays to be used as input to the shaders
		// the two calls to setAttributeBuffer() reference again the 'vertex' buffer whose allocate() function was called last,
		// in this case m_vertexDataBuffer.

		// index 0 = position
		m_program->enableAttributeArray(0); // array with index/id 0
		m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		// index 1 = color
		m_program->enableAttributeArray(1); // array with index/id 1
		m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		m_vao.release();
		// Release (unbind) all
		m_vertexBuffer.release();
		m_elementBuffer.release();
		m_program->release();
	} // end data init

	float * vertexBuffer = m_vertexBufferData.data();
	unsigned int vertexCount = 0;
	GLuint * elementBuffer = m_elementBufferData.data();
	unsigned int elementCount = 0;
	for (unsigned int i=0; i<m_rectangles.size(); ++i) {
		m_rectangles[i].copy2Buffer(vertexBuffer, vertexCount, elementBuffer, elementCount);
		vertexCount += 4;
		elementCount += 2;
	}
	m_vertexBuffer.bind();
	m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_vertexBufferData.size()*sizeof(float);
	m_vertexBuffer.allocate(m_vertexBufferData.data(), vertexMemSize);

	// init grid shader program
	{
		// This is the same stuff as in Var_01, but shortened a bit
		m_gridProgram = new QOpenGLShaderProgram();
		m_gridProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/grid.vert");
		m_gridProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
		m_gridProgram->link();
		m_gridProgram->bind();
		qDebug() << "Errors?" << glGetError() << m_gridProgram->log();

		// Cache Uniform Locations
		u_gridWorldToView = m_gridProgram->uniformLocation("worldToView");
		u_gridColor = m_gridProgram->uniformLocation("gridColor");

		// Create Vertex Array Object
		m_gridVao.create();		// create Vertex Array Object
		m_gridVao.bind();		// sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

		// Create Buffer (Do not release until VAO is created and released)
		m_gridVertexBuffer.create();
		m_gridVertexBuffer.bind();
		m_gridVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		int vertexMemSize = m_gridVertexBufferData.size()*sizeof(float);
		m_gridVertexBuffer.allocate(m_gridVertexBufferData.data(), vertexMemSize);

		// index 0 = position
		m_gridProgram->enableAttributeArray(0); // array with index/id 0
		m_gridProgram->setAttributeBuffer(0, GL_FLOAT,
									  0 /* position/vertex offset */,
									  2 /* two floats per position = vec2 */,
									  0 /* vertex after vertex, no interleaving */);

		m_gridVao.release();
		m_gridVertexBuffer.release();
		m_gridProgram->release();
	}
	// tell OpenGL to show only faces whose normal vector points towards us
	glEnable(GL_CULL_FACE);

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

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


	// monitor rendering - prints out a string whenever the rendering takes place - only for resizing or when
	// focussing in/out of the window
	qDebug() << QDateTime::currentDateTime().toString();

	// clear the background color
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 0
	// render using our shader
	m_program->bind();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	m_program->setUniformValue(u_worldToView, m_worldToView);

	{
		// set the geometry ("position" and "color" arrays)
		m_vao.bind();

		// now draw the cube by drawing individual triangles
		// - GL_TRIANGLES - draw individual triangles via elements
		glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
		// release vertices again
		m_vao.release();
	}
	m_program->release();
#endif
	// *** render grid afterwards ***

	// render using our shader
	m_gridProgram->bind();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	m_gridProgram->setUniformValue(u_worldToView, m_worldToView);
	QVector3D color(0.3f, 0.6f, 0.3f);
	m_gridProgram->setUniformValue(u_gridColor, color);

	{
		// set the geometry ("position" and "color" arrays)
		m_gridVao.bind();

		// now draw the grid lines
		glDrawArrays(GL_LINES, 0, m_gridVertexBufferData.size());
		// release vertices again
		m_gridVao.release();
	}
	m_gridProgram->release();

#if 0

	// clear the background color
	// render using our shader
	SHADER(0)->bind();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	SHADER(0)->setUniformValue(u_worldToView, m_worldToView);
	{
		// set the geometry ("position" and "color" arrays)
		m_boxObject.m_vertexDataBuffer.bind();
		m_boxObject.m_vao.bind();

		// now draw the cube by drawing individual triangles
		// - GL_TRIANGLES - draw individual triangles
		glDrawArrays(GL_TRIANGLES, 0, m_boxObject.m_NVertexes);
		// release vertices again
		m_boxObject.m_vao.release();
	}
	SHADER(0)->release();
#endif
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
//	qDebug() << "m_projection = " << m_projection;
//	qDebug() << "m_camera = " << m_camera.toMatrix();
//	qDebug() << "m_transform = " << transform.toMatrix();
//	qDebug() << "world2View = " << m_worldToView;
	m_needRepaint = true;
}
