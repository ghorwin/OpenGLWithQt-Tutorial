#include "BoxObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>

class Vertex {
public:
	Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector3D &color) :
		m_pos(position), m_col(color)
	{
	}

	QVector3D	m_pos;
	QVector3D	m_col;
};

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

BoxObject::BoxObject()
{

}


void BoxObject::create(QOpenGLShaderProgram * shaderProgramm) {
	m_NVertexes = sizeof(sg_vertexes)/sizeof(sg_vertexes[0]);

	// Create Vertex Array Object
	m_vao.create(); // create underlying OpenGL object
	m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

	// Create Buffer (Do not release until VAO is created)
	m_vertexDataBuffer.create();
	m_vertexDataBuffer.bind();
	m_vertexDataBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexArrayMemSize = sizeof(sg_vertexes);
	m_vertexDataBuffer.allocate(sg_vertexes, vertexArrayMemSize);

	// tell shader program we have two data arrays to be used as input to the shaders
	// the two calls to setAttributeBuffer() reference again the buffer whose allocate() function was called last,
	// in this case m_vertexDataBuffer.
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	// index 0 = position
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
	// index 0 = color
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));

	// Release (unbind) all
	m_vao.release();
	m_vertexDataBuffer.release();
}


