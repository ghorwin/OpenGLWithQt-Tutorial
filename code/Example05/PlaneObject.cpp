/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "PlaneObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>


PlaneObject::PlaneObject() :
	m_vbo(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
	m_ebo(QOpenGLBuffer::IndexBuffer) // make this an Index Buffer
{

#if 0
	QColor col1(255, 0, 0);
	col1.setAlphaF(0.4);
	// add a plane facing towards initial view
	m_planes.push_back( PlaneMesh( QVector3D(-10,0,0), QVector3D(10,0,0), QVector3D(-10,40,0), col1) );
	// add back plane
//	m_planes.push_back( PlaneMesh( QVector3D(10,0,0), QVector3D(-10,0,0), QVector3D(10,40,0), col1) );

#endif


	// create 'some' random vertical planes

	const int PlaneCount = 10; // DISCUSS
	const float dim = 500;
	const float planeHeight = 10;
	const float planeAlpha = 0.7;

	for (unsigned int i=0; i<PlaneCount; ++i) {
		// randomize x and z coordinates
		float x1 = qrand()*double(dim)/RAND_MAX - dim/2;
		float x2 = qrand()*double(dim)/RAND_MAX - dim/2;
		float z1 = qrand()*double(dim)/RAND_MAX - dim/2;
		float z2 = qrand()*double(dim)/RAND_MAX - dim/2;

		// we now generate a plane with these coordinates
		QVector3D a(x1,0, z1);
		QVector3D b(x2,0, z2);
		QVector3D d(x1,planeHeight, z1);

		float red = qrand()*256./RAND_MAX;
		float green = qrand()*256./RAND_MAX;
		float blue = qrand()*256./RAND_MAX;

		QColor col(red,green,blue,planeAlpha*255);

		m_planes.push_back( PlaneMesh(a,b,d, col));
	}

	unsigned int N = m_planes.size();

	// resize storage arrays
	m_vertexBufferData.resize(N*PlaneMesh::VertexCount);
	m_elementBufferData.resize(N*PlaneMesh::IndexCount);

	// update the buffers
	VertexVCA * vertexBuffer = m_vertexBufferData.data();
	unsigned int vertexCount = 0;
	GLuint * elementBuffer = m_elementBufferData.data();
	for (const PlaneMesh & p : m_planes)
		p.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
}


void PlaneObject::create(QOpenGLShaderProgram * shaderProgramm) {
	// create and bind Vertex Array Object
	m_vao.create();
	m_vao.bind();

	// create and bind vertex buffer
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_vertexBufferData.size()*sizeof(VertexVCA);
	qDebug() << "PlaneObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
	m_vbo.allocate(m_vertexBufferData.data(), vertexMemSize);

	// create and bind element buffer
	m_ebo.create();
	m_ebo.bind();
	m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
	qDebug() << "PlaneObject - ElementBuffer size =" << elementMemSize/1024.0 << "kByte";
	m_ebo.allocate(m_elementBufferData.data(), elementMemSize);

	// set shader attributes
	// tell shader program we have two data arrays to be used as input to the shaders

	// index 0 = position
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 4, sizeof(VertexVCA));
	// index 1 = color
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(VertexVCA, r), 4, sizeof(VertexVCA));

	// Release (unbind) all
	m_vao.release();
	m_vbo.release();
	m_ebo.release();
}


void PlaneObject::destroy() {
	m_vao.destroy();
	m_vbo.destroy();
	m_ebo.destroy();
}


void PlaneObject::render() {
	// set the geometry ("position", "normal" and "color" arrays)
	m_vao.bind();

	// now draw the cube by drawing individual triangles
	// - GL_TRIANGLES - draw individual triangles via elements
	glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
	// release vertices again
	m_vao.release();
}

