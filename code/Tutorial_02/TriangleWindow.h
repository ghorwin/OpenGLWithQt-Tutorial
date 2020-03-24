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
#include <QOpenGLWindow>

/*	This is the window that shows the triangle.
	We derive from our QOpenGLWindow base class and implement the
	virtual initializeGL() and paintGL() functions.
*/
class TriangleWindow : public QOpenGLWindow {
public:
	TriangleWindow();
	virtual ~TriangleWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	// Wraps an OpenGL VertexArrayObject (VAO)
	QOpenGLVertexArrayObject	m_vao;
	// Vertex buffer (positions and colors, interleaved storage mode).
	QOpenGLBuffer				m_vertexBufferObject;
	// Index buffer to draw two rectangles
	QOpenGLBuffer				m_indexBufferObject;

	// Holds the compiled shader programs.
	QOpenGLShaderProgram		*m_program;
};

#endif // TriangleWindow_H
