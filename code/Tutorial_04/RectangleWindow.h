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
#include <QOpenGLWidget>

/*	This is the window that shows the two triangles to form a rectangle.
	We derive from our QOpenGLWidget base class and implement the
	virtual initializeGL() and paintGL() functions.
*/
class RectangleWindow : public QOpenGLWidget {
public:
	RectangleWindow(QWidget * parent = nullptr);
	virtual ~RectangleWindow() Q_DECL_OVERRIDE;

	// updates the scene and requests a paint update
	void updateScene();

	void animateColorsTo(const std::vector<QColor> & toColors);

	// holds the vertex colors set on next call to updateScene()
	std::vector<QColor>			m_vertexColors;

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	// modifies the scene a bit and call updateScene() afterwards
	// when already in the final state, doesn't do anything
	void animate();

	// Wraps an OpenGL VertexArrayObject (VAO)
	QOpenGLVertexArrayObject	m_vao;
	// Vertex buffer (positions and colors, interleaved storage mode).
	QOpenGLBuffer				m_vertexBufferObject;
	// Index buffer to draw two rectangles
	QOpenGLBuffer				m_indexBufferObject;

	// Holds the compiled shader programs.
	QOpenGLShaderProgram		*m_program;

	std::vector<float>			m_vertexBufferData;

	// Stores the target colors that we animate towards
	std::vector<QColor>			m_toColors;
	// Stores the target colors that we animate from
	std::vector<QColor>			m_fromColors;
	// number of frames used for the animation
	unsigned int				m_frameCount;
};

#endif // RectangleWindow_H
