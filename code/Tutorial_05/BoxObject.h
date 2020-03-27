/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

#include "RectMesh.h"

class BoxObject {
public:
	BoxObject();

	/*! The function is called during OpenGL initialization, where the OpenGL context is current. */
	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	void render();

	std::vector<RectMesh>		m_rectangles;

	std::vector<float>			m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_vao;

	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vertexBuffer;
	/*! Holds elements. */
	QOpenGLBuffer				m_elementBuffer;
};

#endif // BOXOBJECT_H
