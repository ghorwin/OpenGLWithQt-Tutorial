#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "OpenGLWindow.h"

/*! This is the window that shows the triangle.
	We derive from our OpenGLWindow base class and implement the
	virtual initialize() and render() functions.
*/
class TriangleWindow : public OpenGLWindow {
public:
	TriangleWindow();
	~TriangleWindow();

	void initialize() Q_DECL_OVERRIDE;
	void render() Q_DECL_OVERRIDE;

private:

	/*! Wraps an OpenGL VertexArrayObject, that holds the vertex buffer. */
	QOpenGLVertexArrayObject	m_vao;
	/*! Vertex buffer (only positions now). */
	QOpenGLBuffer				m_vertexBufferObject;

	/*! Holds the compiled shader programs. */
	QOpenGLShaderProgram		*m_program;
};

#endif // TRIANGLEWINDOW_H
