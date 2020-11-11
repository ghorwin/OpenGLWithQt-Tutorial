/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef RectangleWindow_H
#define RectangleWindow_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWindow>

/*	This is the window that shows the two triangles to form a rectangle.
	We derive from our QOpenGLWindow base class and implement the
	virtual initializeGL() and paintGL() functions.
*/
class RectangleWindow : public QOpenGLWindow {
public:
	RectangleWindow();
	virtual ~RectangleWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	// Wraps an OpenGL VertexArrayObject (VAO)
	QOpenGLVertexArrayObject	m_vao;
	// Vertex buffer (positions and colors, interleaved storage mode).
	// Holds 4 vertices of the rectangle
	QOpenGLBuffer				m_vertexBufferObject;
	QOpenGLBuffer				m_colorBufferObject;
	// Index buffer to draw two rectangles in triangle strip mode
	QOpenGLBuffer				m_indexBufferObject;

	unsigned int				m_indexCount;

	// Holds the compiled shader programs.
	QOpenGLShaderProgram		*m_program;
};

#endif // RectangleWindow_H
