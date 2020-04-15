/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef Texture2ScreenObject_H
#define Texture2ScreenObject_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

/*! Paints a texture to the screen - no transformation, just dumping the texture to the screen.
	Optionally, image kernels are used in fragment shader.
*/
class Texture2ScreenObject {
public:
	/*! The function is called during OpenGL initialization, where the OpenGL context is current. */
	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	/*! Binds the buffer and paints. Expects the texture to be bound already. */
	void render();

	QOpenGLVertexArrayObject	m_vao;
	QOpenGLBuffer				m_vbo;
};

#endif // Texture2ScreenObject_H
