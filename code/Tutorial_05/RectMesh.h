#ifndef RECTMESH_H
#define RECTMESH_H

#include <QColor>
#include <QVector3D>
#include <QtGui/qopengl.h>

/*! A simple class that generates a rectangle mesh.
	The rectangle is defined by 3 points in space, a, b and c, where
	ab and ac are orthogonal vectors, and the cross-product of a, b, and c
	points towards the viewer (face normal).
*/
class RectMesh {
public:
	RectMesh() {}
	RectMesh(   QVector3D a,
				QVector3D b,
				QVector3D c,
				QColor color) :
		m_a(a), m_b(b), m_c(c), m_color(color.redF(), color.greenF(), color.blueF())
	{}

	/*! Fills in vertex data in a buffer, provided by the caller.
		The data is stored interleaved, basically registering 4 vertexes.
		The vertex buffer requires the size 4*sizeof(Vertex) and the element buffer
		requires 2*3 GLunit.

		vertexCount is the number of vertexes already in the vertexBuffer (each call to copy2Buffer adds 4)
		elementCount is the number of elements already in the elementsBuffer (each call to copy2Buffer adds 2)
	*/
	void copy2Buffer(float * vertexBuffer, unsigned int vertexCount,
					GLuint * elementBuffer, unsigned int elementCount);

	QVector3D m_a;
	QVector3D m_b;
	QVector3D m_c;
	QVector3D m_color;
};

#endif // RECTMESH_H
