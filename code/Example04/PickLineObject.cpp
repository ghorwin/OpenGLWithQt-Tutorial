/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "PickLineObject.h"

#include <QOpenGLShaderProgram>
#include <vector>

void PickLineObject::create(QOpenGLShaderProgram * shaderProgramm) {
	// create a temporary buffer that will contain the x-z coordinates of all grid lines
	// we have 1 line, with two vertexes, with 2xthree floats (position and color)
	m_vertexBufferData.resize(2);
	m_vertexBufferData[0] = Vertex(QVector3D(5,5,5), Qt::white);
	m_vertexBufferData[1] = Vertex(QVector3D(-5,-5,-5), Qt::red);

	// Create Vertex Array Object
	m_vao.create();		// create Vertex Array Object
	m_vao.bind();		// and bind it

	// Create Vertex Buffer Object
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_vertexBufferData.size()*sizeof(Vertex);
	m_vbo.allocate(m_vertexBufferData.data(), vertexMemSize);

	// index 0 = position
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
	// index 1 = color
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, r), 3, sizeof(Vertex));

	m_vao.release();
	m_vbo.release();
}


void PickLineObject::destroy() {
	m_vao.destroy();
	m_vbo.destroy();
}


void PickLineObject::render() {
	m_vao.bind();
	glDrawArrays(GL_LINES, 0, m_vertexBufferData.size());
	m_vao.release();
}


void PickLineObject::setPoints(const QVector3D & a, const QVector3D & b) {
	m_vertexBufferData[0] = Vertex(a, Qt::white);
	m_vertexBufferData[1] = Vertex(b, QColor(64,0,0));
	int vertexMemSize = m_vertexBufferData.size()*sizeof(Vertex);
	m_vbo.bind();
	m_vbo.allocate(m_vertexBufferData.data(), vertexMemSize);
	m_vbo.release();
	m_visible = true;
}

