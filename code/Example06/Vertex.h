/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QColor>

/*! A container class to store data (coordinates, normals, textures, colors) of a vertex, used for interleaved
	storage. Expand this class as needed.

	Memory layout (each char is a byte): xxxxyyyyzzzzrrrrggggbbbb = 6*4 = 24 Bytes

	You can define a vector<Vertex> and use this directly as input to the vertex buffer.

	Mind implicit padding by compiler! Hence, for allocation use:
	- sizeof(Vertex) as stride
	- offsetof(Vertex, r) as start offset for the color

	This will only become important, if mixed data types are used in the struct.
	Read http://www.catb.org/esr/structure-packing/ for an in-depth explanation.
*/
struct Vertex {
	Vertex() {}
	Vertex(const QVector3D & coords, const QColor & col) :
		x(float(coords.x())),
		y(float(coords.y())),
		z(float(coords.z())),
		r(float(col.redF())),
		g(float(col.greenF())),
		b(float(col.blueF()))
	{
	}

	float x,y,z;
	float r,g,b;
};

/*! A container class to store data (coordinates, normals, textures, colors) of a vertex, used for interleaved
	storage. Expand this class as needed.

	Memory layout (each char is a byte): xxxxyyyyzzzznnnnmmmmoooorrrrggggbbbb = 9*4 = 36 Bytes

	(nmo = normal vector components)

	You can define a vector<VertexVNC> and use this directly as input to the vertex buffer.
*/
struct VertexVNC {
	VertexVNC() {}
	VertexVNC(const QVector3D & coords, const QVector3D & normal, const QColor & col) :
		x(float(coords.x())),
		y(float(coords.y())),
		z(float(coords.z())),
		m(float(normal.x())),
		n(float(normal.y())),
		o(float(normal.z())),
		r(float(col.redF())),
		g(float(col.greenF())),
		b(float(col.blueF()))
	{
	}

	float x,y,z;
	float m,n,o;
	float r,g,b;
};


/*! A container class to store data (coordinates, normals, textures, colors) of a vertex, used for interleaved
	storage. Expand this class as needed.

	Memory layout (each char is a byte): xxxxyyyyzzzzwwwwrrrrggggbbbbaaaa = 8*4 = 32 Bytes

	(nmo = normal vector components)

	You can define a vector<VertexVNC> and use this directly as input to the vertex buffer.
*/
struct VertexVCA {
	VertexVCA() {}
	VertexVCA(const QVector3D & coords, const QColor & col) :
		x(float(coords.x())),
		y(float(coords.y())),
		z(float(coords.z())),
		w(0),
		r(float(col.redF())),
		g(float(col.greenF())),
		b(float(col.blueF())),
		a(float(col.alphaF()))
	{
	}

	float x,y,z,w;
	float r,g,b,a;
};


struct VertexTex {
	VertexTex() {}
	VertexTex(const QVector3D & coords, float texi_, float texj_) :
		x(float(coords.x())),
		y(float(coords.y())),
		z(float(coords.z())),
		texi(texi_),
		texj(texj_)
	{
	}

	float x,y,z;
	float texi,texj;
};


#endif // VERTEX_H
