/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "BoxObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>

BoxObject::BoxObject() :
	m_vertexBuffer(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
	m_elementBuffer(QOpenGLBuffer::IndexBuffer)
{

	// create a box
	BoxMesh b(5,2,3);
	b.setFaceColors({Qt::blue, Qt::red, Qt::yellow, Qt::green, Qt::magenta, Qt::darkCyan});
	m_boxes.push_back( b);

	// n count
	unsigned int NCubes = m_boxes.size();

	// we have 6 sides of a cube, and each side needs 4 vertexes
	m_vertexBufferData.resize(NCubes*BoxMesh::VertexCount);

	// we have 6 sides of cube, and each side has two triangles, with 3 indexes each
	m_elementBufferData.resize(NCubes*6*BoxMesh::IndexCount);

	// update the buffers
	Vertex * vertexBuffer = m_vertexBufferData.data();
	unsigned int vertexCount = 0;
	GLuint * elementBuffer = m_elementBufferData.data();
	unsigned int elementCount = 0;
	for (unsigned int i=0; i<m_boxes.size(); ++i) {
		m_boxes[i].copy2Buffer(vertexBuffer, m_vertexBufferData.size()-vertexCount, elementBuffer, m_elementBufferData.size()-elementCount, vertexCount);
		vertexCount += BoxMesh::VertexCount;
		elementCount += BoxMesh::IndexCount;
	}
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
	int vertexMemSize = m_vertexBufferData.size()*sizeof(Vertex);
	m_vertexBuffer.allocate(m_vertexBufferData.data(), vertexMemSize);

	m_elementBuffer.create();
	m_elementBuffer.bind();
	m_elementBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
	m_elementBuffer.allocate(m_elementBufferData.data(), elementMemSize);

	// Create Shader (Do not release until VAO is created)

	// tell shader program we have two data arrays to be used as input to the shaders
	// the two calls to setAttributeBuffer() reference again the 'vertex' buffer whose allocate() function was called last,
	// in this case m_vertexDataBuffer.

	// index 0 = position
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
	// index 1 = color
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, r), 3, sizeof(Vertex));

	m_vao.release();
	// Release (unbind) all
	m_vertexBuffer.release();
	m_elementBuffer.release();
}


void BoxObject::destroy() {
	m_vao.destroy();
	m_vertexBuffer.destroy();
	m_elementBuffer.destroy();
}


void BoxObject::render() {
	// set the geometry ("position" and "color" arrays)
	m_vao.bind();

	// now draw the cube by drawing individual triangles
	// - GL_TRIANGLES - draw individual triangles via elements
	glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
	// release vertices again
	m_vao.release();
}
