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

#include "BoxMesh.h"

struct PickObject;

/*! A container for all the boxes.
	Basically creates the geometry of the individual boxes and populates the buffers.
*/
class BoxObject {
public:
	BoxObject();

	/*! The function is called during OpenGL initialization, where the OpenGL context is current. */
	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	void render();

	/*! Thread-save pick function.
		Checks if any of the box object surfaces is hit by the ray defined by "p1 + d [0..1]" and
		stores data in po (pick object).
	*/
	void pick(const QVector3D & p1, const QVector3D & d, PickObject & po) const;

	/*! Changes color of box and face to show that the box was clicked on. */
	void highlight(unsigned int boxId, unsigned int faceId);

	std::vector<BoxMesh>		m_boxes;

	std::vector<Vertex>			m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_vao;

	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vbo;
	/*! Holds elements. */
	QOpenGLBuffer				m_ebo;
};

#endif // BOXOBJECT_H
