/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "PlaneMesh.h"
#include "PickObject.h"

void copyPlane2Buffer(VertexVCA * & vertexBuffer, GLuint * & elementBuffer, unsigned int & elementStartIndex,
					  const VertexVCA & a, const VertexVCA & b, const VertexVCA & c, const VertexVCA & d)
{
	// first store the vertex data (a,b,c,d in counter-clockwise order)

	vertexBuffer[0] = a;
	vertexBuffer[1] = b;
	vertexBuffer[2] = c;
	vertexBuffer[3] = d;

	// advance vertexBuffer
	vertexBuffer += 4;

	// we generate data for two triangles: a, b, d  and b, c, d

	elementBuffer[0] = elementStartIndex;
	elementBuffer[1] = elementStartIndex+1;
	elementBuffer[2] = elementStartIndex+3;
	elementBuffer[3] = elementStartIndex+1;
	elementBuffer[4] = elementStartIndex+2;
	elementBuffer[5] = elementStartIndex+3;

	// advance elementBuffer
	elementBuffer += 6;
	// 4 vertices have been added, so increase start number for next plane
	elementStartIndex += 4;
}


void PlaneMesh::copy2Buffer(VertexVCA *& vertexBuffer, GLuint *& elementBuffer, unsigned int & elementStartIndex) const {

	// Compute point c
	QVector3D c = (m_b-m_a) + m_d;

	// push into vertex memory a, b, c, d, vertexes (0, 1, 2, 3)
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			VertexVCA(m_a, m_color),
			VertexVCA(m_b, m_color),
			VertexVCA(c, m_color),
			VertexVCA(m_d, m_color)
		);
}






