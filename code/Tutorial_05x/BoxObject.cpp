#include "BoxObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>

BoxObject::BoxObject() :
	m_vertexBuffer(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
	m_elementBuffer(QOpenGLBuffer::IndexBuffer)
{

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
}


void BoxObject::create(QOpenGLShaderProgram * shaderProgramm) {
	// Create Shader (Do not release until VAO is created)

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
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6*sizeof(float));
	// index 1 = color
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 6*sizeof(float));

	m_vao.release();
	// Release (unbind) all
	m_vertexBuffer.release();
	m_elementBuffer.release();
}


