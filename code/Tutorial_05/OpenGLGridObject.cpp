#include "OpenGLGridObject.h"

#include <QOpenGLShaderProgram>
#include <vector>

OpenGLGridObject::OpenGLGridObject()
{

}


void OpenGLGridObject::create(QOpenGLShaderProgram * shaderProgramm) {
	// create the grid lines

	const unsigned int N = 100; // number of lines to draw in x and z direction
	float width = 500; // grid is centered around origin, and expands to width/2 in -x, +x, -z and +z direction
	// width is in "space units", whatever that means for you (meters, km, nanometers...)

	// create a temporary buffer that will contain the x-z coordinates of all grid lines
	std::vector<float>			gridVertexBufferData;
	// we have 2*N lines, each line requires two vertexes, with two floats (x and z coordinates) each.
	m_NVertexes = 2*N*2*2;
	gridVertexBufferData.resize(m_NVertexes);
	float * gridVertexBufferPtr = gridVertexBufferData.data();
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

	// Create Vertex Array Object
	m_vao.create();		// create Vertex Array Object
	m_vao.bind();		// sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

	// Create Buffer (Do not release until VAO is created and released)
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_NVertexes*sizeof(float);
	m_vbo.allocate(gridVertexBufferData.data(), vertexMemSize);

	// layout(location = 0) = vec2 position
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT,
								  0 /* position/vertex offset */,
								  2 /* two floats per position = vec2 */,
								  0 /* vertex after vertex, no interleaving */);

	m_vao.release();
	m_vbo.release();
}
