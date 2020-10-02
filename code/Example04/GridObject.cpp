/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "GridObject.h"

#include <QOpenGLShaderProgram>
#include <vector>


void GridObject::create(QOpenGLShaderProgram * shaderProgramm, bool major) {
	// grid is centered around origin, and expands to width/2 in -x, +x, -z and +z direction

	// create a temporary buffer that will contain the x-z coordinates of all grid lines
	std::vector<float>			gridVertexBufferData;
	// we have at max 2*N lines, each line requires two vertexes, with two floats (x and z coordinates) each.
	// reserve memory, but actual buffer size depends on number of lines added
	gridVertexBufferData.reserve(2*m_N*2*2);	// DISCUSS
//	float * gridVertexBufferPtr = gridVertexBufferData.data();
	// compute grid lines with z = const
	float x1 = -m_width*0.5;
	float x2 = m_width*0.5;
	for (unsigned int i=0; i<m_N; ++i) {
		// in major grid mode, we only add every 10th line
		bool majorGridLine = (i % 10 == 0);
		// skip lines not matching the grid
		if (major && !majorGridLine) continue;
		if (!major && majorGridLine) continue;
		float z = m_width/(m_N-1)*i-m_width*0.5;
		gridVertexBufferData.push_back(x1);
		gridVertexBufferData.push_back(z);
		gridVertexBufferData.push_back(x2);
		gridVertexBufferData.push_back(z);
	}
	// compute grid lines with x = const
	float z1 = -m_width*0.5;
	float z2 = m_width*0.5;
	for (unsigned int i=0; i<m_N; ++i) {
		// in major grid mode, we only add every 10th line
		bool majorGridLine = (i % 10 == 0);
		// skip lines not matching the grid
		if (major && !majorGridLine) continue;
		if (!major && majorGridLine) continue;
		float x = m_width/(m_N-1)*i-m_width*0.5;
		gridVertexBufferData.push_back(x);
		gridVertexBufferData.push_back(z1);
		gridVertexBufferData.push_back(x);
		gridVertexBufferData.push_back(z2);
	}

	m_bufferSize = gridVertexBufferData.size();

	// Create Vertex Array Object
	m_vao.create();		// create Vertex Array Object
	m_vao.bind();		// and bind it

	// Create Vertex Buffer Object
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_bufferSize*sizeof(float);
	qDebug() << "GridObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
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


void GridObject::destroy() {
	m_vao.destroy();
	m_vbo.destroy();
}


void GridObject::render() {
	m_vao.bind();
	// draw the grid lines, m_NVertexes = number of floats in buffer
	glDrawArrays(GL_LINES, 0, m_bufferSize);
	m_vao.release();
}
