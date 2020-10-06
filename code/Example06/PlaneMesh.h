/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef PlaneMeshH
#define PlaneMeshH

#include <QtGui/QOpenGLFunctions>

#include <QColor>

#include "Vertex.h"

/*! A mesh for rectangular planes.
	Since GL_QUADS is deprecated, we always draw planes as two triangles.
*/
class PlaneMesh {
public:
	PlaneMesh() {}
	PlaneMesh(QVector3D a, QVector3D b, QVector3D d, QColor color = Qt::blue) :
		m_a(a), m_b(b), m_d(d), m_color(color) {}

	/*! Fills in vertex data in a buffer, provided by the caller.
		The vertex data is stored interleaved, "coordinates(vec4)-color(vec4)-coordinates(vec4)-...".

		\param vertexBuffer Pointer to vertex memory array to write into. Will be moved forward to point to the next
			position after the inserted vertices.
		\param elementBuffer Pointer to element memory array to write into. Will be moved forward to point to the next
			index position after the inserted vertices.

		elementStartIndex is the start index, that we should start indexing our newly added vertexes with.
	*/
	void copy2Buffer(VertexVCA * & vertexBuffer,
					GLuint * & elementBuffer,
					unsigned int & elementStartIndex) const;

	void copy2Buffer(VertexTex * & vertexBuffer,
					GLuint * & elementBuffer,
					unsigned int & elementStartIndex) const;

	static const unsigned int VertexCount = 4;
	static const unsigned int IndexCount = 6;

	float		m_texi1;
	float		m_texj1;
	float		m_texi2;
	float		m_texj2;

private:
	QVector3D	m_a;
	QVector3D	m_b;
	QVector3D	m_d;
	QColor		m_color;
};

#endif // PlaneMeshH
