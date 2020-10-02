/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef OPENGLGRIDOBJECT_H
#define OPENGLGRIDOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE


/*! This class holds all data needed to draw a grid on the screen.
	We have only a coordinate buffer, which holds tightly packed 2 * vec2 (start and end points of lines, xz coords)
	with y=0 implied.
	Grid color is a uniform, as is background color.

	The grid is drawn with the grid shader program, which is passed to the create() function.
*/
class GridObject {
public:
	/*! The function is called during OpenGL initialization, where the OpenGL context is current.
		major - if true, only the major grid lines are generated, if false, all the minor grid lines
		except the major lines are createds
	*/
	void create(QOpenGLShaderProgram * shaderProgramm, bool major);
	void destroy();

	/*! Binds the buffer and paints. */
	void render();

	/*! Cached size of vertex buffer object. */
	unsigned int				m_bufferSize;

	// number of lines to draw in x and z direction
	const unsigned int			m_N = 1000;
	// width is in "space units", whatever that means for you (meters, km, nanometers...)
	const float					m_width = 5000;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates. */
	QOpenGLVertexArrayObject	m_vao;
	/*! Holds positions of grid lines. */
	QOpenGLBuffer				m_vbo;

};

#endif // OPENGLGRIDOBJECT_H
