/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "RectMesh.h"

void RectMesh::copy2Buffer(float * vertexBuffer, unsigned vertexCount,
						   GLuint * elementBuffer, unsigned int elementCount)
{
	// compute coordinates of point d
	QVector3D ab = m_b - m_a;
	QVector3D d = m_c + ab;
	// now store the vertex coordinates in counter clock-wise order: a,b,c,d
	// then the elements a, b, c and a, b, d
	vertexBuffer += vertexCount*6;
	*vertexBuffer     = m_a.x();
	*(++vertexBuffer) = m_a.y();
	*(++vertexBuffer) = m_a.z();
	*(++vertexBuffer) = m_color.x();
	*(++vertexBuffer) = m_color.y();
	*(++vertexBuffer) = m_color.z();

	*(++vertexBuffer) = m_b.x();
	*(++vertexBuffer) = m_b.y();
	*(++vertexBuffer) = m_b.z();
	*(++vertexBuffer) = m_color.x();
	*(++vertexBuffer) = m_color.y();
	*(++vertexBuffer) = m_color.z();

	*(++vertexBuffer) = m_c.x();
	*(++vertexBuffer) = m_c.y();
	*(++vertexBuffer) = m_c.z();
	*(++vertexBuffer) = m_color.x();
	*(++vertexBuffer) = m_color.y();
	*(++vertexBuffer) = m_color.z();

	*(++vertexBuffer) = d.x();
	*(++vertexBuffer) = d.y();
	*(++vertexBuffer) = d.z();
	*(++vertexBuffer) = m_color.x();
	*(++vertexBuffer) = m_color.y();
	*(++vertexBuffer) = m_color.z();


	// also populate element buffer
	elementBuffer += elementCount*3;
	*elementBuffer     = vertexCount;    // a
	*(++elementBuffer) = vertexCount+1;  // b
	*(++elementBuffer) = vertexCount+2;  // c
	*(++elementBuffer) = vertexCount+1;  // b
	*(++elementBuffer) = vertexCount+3;  // b
	*(++elementBuffer) = vertexCount+2;  // c
}
