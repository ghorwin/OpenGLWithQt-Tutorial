/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef TriangleWindow_H
#define TriangleWindow_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "OpenGLWindow.h"

/*	This is the window that shows the triangle.
	We derive from our OpenGLWindow base class and implement the
	virtual initialize() and render() functions.
*/
class TriangleWindow : public OpenGLWindow {
public:
	TriangleWindow();

	void initialize() Q_DECL_OVERRIDE;
	void render() Q_DECL_OVERRIDE;

protected:

	// overloaded to catch close events
	virtual bool event(QEvent *ev) Q_DECL_OVERRIDE;

private:
	// Wraps an OpenGL VertexArrayObject (VAO)
	QOpenGLVertexArrayObject	m_vao;
	// Vertex buffer (only positions now).
	QOpenGLBuffer				m_vertexBufferObject;

	// Holds the compiled shader programs.
	QOpenGLShaderProgram		*m_program;
};

#endif // TriangleWindow_H
