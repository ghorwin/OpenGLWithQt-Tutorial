/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "BoxMesh.h"
#include "PickObject.h"

void copyPlane2Buffer(Vertex * & vertexBuffer, GLuint * & elementBuffer, unsigned int & elementStartIndex,
					  const Vertex & a, const Vertex & b, const Vertex & c, const Vertex & d);


BoxMesh::BoxMesh(float width, float height, float depth, QColor boxColor) {

	m_vertices.push_back(QVector3D(-0.5f*width, -0.5f*height,  0.5f*depth)); // a = 0
	m_vertices.push_back(QVector3D( 0.5f*width, -0.5f*height,  0.5f*depth)); // b = 1
	m_vertices.push_back(QVector3D( 0.5f*width,  0.5f*height,  0.5f*depth)); // c = 2
	m_vertices.push_back(QVector3D(-0.5f*width,  0.5f*height,  0.5f*depth)); // d = 3

	m_vertices.push_back(QVector3D(-0.5f*width, -0.5f*height, -0.5f*depth)); // e = 4
	m_vertices.push_back(QVector3D( 0.5f*width, -0.5f*height, -0.5f*depth)); // f = 5
	m_vertices.push_back(QVector3D( 0.5f*width,  0.5f*height, -0.5f*depth)); // g = 6
	m_vertices.push_back(QVector3D(-0.5f*width,  0.5f*height, -0.5f*depth)); // h = 7

	setColor(boxColor);
}


void BoxMesh::transform(const QMatrix4x4 & transform) {
	for (QVector3D & v : m_vertices)
		v = transform*v;
}


bool BoxMesh::intersects(unsigned int planeIdx, const QVector3D & p1, const QVector3D & d, float & z) const {
	const Rect & p = m_planeInfo[planeIdx];
	return intersectsPlane(p.m_a, p.m_b, p.m_normal, p.m_offset, p1, d, z);
}


void BoxMesh::copy2Buffer(Vertex *& vertexBuffer, GLuint *& elementBuffer, unsigned int & elementStartIndex) const {
	std::vector<QColor> cols;
	Q_ASSERT(!m_colors.empty());
	// three ways to store vertex colors
	if (m_colors.size() == 1) {
		cols = std::vector<QColor>(6, m_colors[0]);
	}
	else {
		Q_ASSERT(m_colors.size() == 6);
		cols = m_colors;
	}

	// now we populate the vertex buffer for all planes

	// front plane: a, b, c, d, vertexes (0, 1, 2, 3)
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			Vertex(m_vertices[0], cols[0]),
			Vertex(m_vertices[1], cols[0]),
			Vertex(m_vertices[2], cols[0]),
			Vertex(m_vertices[3], cols[0])
		);

	// right plane: b=1, f=5, g=6, c=2, vertexes
	// Mind: colors are numbered up
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			Vertex(m_vertices[1], cols[1]),
			Vertex(m_vertices[5], cols[1]),
			Vertex(m_vertices[6], cols[1]),
			Vertex(m_vertices[2], cols[1])
		);

	// back plane: g=5, e=4, h=7, g=6
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			Vertex(m_vertices[5], cols[2]),
			Vertex(m_vertices[4], cols[2]),
			Vertex(m_vertices[7], cols[2]),
			Vertex(m_vertices[6], cols[2])
		);

	// left plane: 4,0,3,7
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			Vertex(m_vertices[4], cols[3]),
			Vertex(m_vertices[0], cols[3]),
			Vertex(m_vertices[3], cols[3]),
			Vertex(m_vertices[7], cols[3])
		);

	// bottom plane: 4,5,1,0
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			Vertex(m_vertices[4], cols[4]),
			Vertex(m_vertices[5], cols[4]),
			Vertex(m_vertices[1], cols[4]),
			Vertex(m_vertices[0], cols[4])
		);

	// top plane: 3,2,6,7
	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
			Vertex(m_vertices[3], cols[5]),
			Vertex(m_vertices[2], cols[5]),
			Vertex(m_vertices[6], cols[5]),
			Vertex(m_vertices[7], cols[5])
		);

	// compute all face normals
	std::vector<Rect> & planeInfo = const_cast<std::vector<Rect> &>(m_planeInfo);
	planeInfo.resize(6);
	// front plane: a, b, c, d, vertexes (0, 1, 2, 3)
	planeInfo[0] = Rect(m_vertices[0], m_vertices[1], m_vertices[3]);
	// right plane: b=1, f=5, g=6, c=2, vertexes
	planeInfo[1] = Rect(m_vertices[1], m_vertices[5], m_vertices[2]);
	// back plane: g=5, e=4, h=7, g=6
	planeInfo[2] = Rect(m_vertices[5], m_vertices[4], m_vertices[6]);
	// left plane: 4,0,3,7
	planeInfo[3] = Rect(m_vertices[4], m_vertices[0], m_vertices[7]);
	// bottom plane: 4,5,1,0
	planeInfo[4] = Rect(m_vertices[4], m_vertices[5], m_vertices[0]);
	// top plane: 3,2,6,7
	planeInfo[5] = Rect(m_vertices[3], m_vertices[2], m_vertices[7]);
}


BoxMesh::Rect::Rect(QVector3D a, QVector3D b, QVector3D d) {
	m_a = b-a;
	m_b = d-a;
	m_normal = QVector3D::crossProduct(m_a, m_b);
	Q_ASSERT(m_normal.length() != 0.f);
	m_normal.normalize();
	m_offset = a;
}

void copyPlane2Buffer(Vertex * & vertexBuffer, GLuint * & elementBuffer, unsigned int & elementStartIndex,
					  const Vertex & a, const Vertex & b, const Vertex & c, const Vertex & d)
{
	// first store the vertex data (a,b,c,d in counter-clockwise order)

	vertexBuffer[0] = a;
	vertexBuffer[1] = b;
	vertexBuffer[2] = c;
	vertexBuffer[3] = d;

	// tweak the colors of the bottom left and bottom right nodes
	if (a.y < c.y) {
		vertexBuffer[0].r *= 0.5;
		vertexBuffer[1].r *= 0.5;
		vertexBuffer[0].g *= 0.5;
		vertexBuffer[1].g *= 0.5;
		vertexBuffer[0].b *= 0.5;
		vertexBuffer[1].b *= 0.5;
	}

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




