/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef PlaneObjectH
#define PlaneObjectH

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

#include "PlaneMesh.h"

/*! A container for transparent planes.
*/
class PlaneObject {
public:
	PlaneObject();

	/*! The function is called during OpenGL initialization, where the OpenGL context is current. */
	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	void render();

	std::vector<PlaneMesh>		m_planes;

	std::vector<VertexVCA>		m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_vao;

	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vbo;
	/*! Holds elements. */
	QOpenGLBuffer				m_ebo;
};

#endif // PlaneObjectH
