/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "BoxObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

BoxObject::BoxObject() :
	m_vbo(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
	m_ebo(QOpenGLBuffer::IndexBuffer) // make this an Index Buffer
{

	// create first box
	BoxMesh b(4,2,3);
	b.setFaceColors({Qt::blue, Qt::red, Qt::yellow, Qt::green, Qt::magenta, Qt::darkCyan});
	Transform3D trans;
	trans.setTranslation(0,1,0);
	b.transform(trans.toMatrix());
	m_boxes.push_back( b);

	// create 'some' other boxes

	const int BoxGenCount = 0;
	const int GridDim = 50; // must be an int, or you have to use a cast below

	// initialize grid (block count)
	int boxPerCells[GridDim][GridDim];
	for (unsigned int i=0; i<GridDim; ++i)
		for (unsigned int j=0; j<GridDim; ++j)
			boxPerCells[i][j] = 0;
	for (unsigned int i=0; i<BoxGenCount; ++i) {
		// create other boxes in randomize grid, x and z dimensions fixed, height varies discretely
		// x and z translation in a grid that has dimension 'GridDim' with 5 space units as grid (line) spacing
		int xGrid = qrand()*double(GridDim)/RAND_MAX;
		int zGrid = qrand()*double(GridDim)/RAND_MAX;
		int boxCount = boxPerCells[xGrid][zGrid]++;
		float boxHeight = 4.5;
		BoxMesh b(4,boxHeight,3);
		b.setFaceColors({Qt::blue, Qt::red, Qt::yellow, Qt::green, Qt::magenta, Qt::darkCyan});
		trans.setTranslation((-GridDim/2+xGrid)*5, boxCount*5 + 0.5*boxHeight, (-GridDim/2 + zGrid)*5);
		b.transform(trans.toMatrix());
		m_boxes.push_back(b);
	}

	unsigned int NBoxes = m_boxes.size();

	// resize storage arrays
	m_vertexBufferData.resize(NBoxes*BoxMesh::VertexCount);
	m_elementBufferData.resize(NBoxes*BoxMesh::IndexCount);

	// update the buffers
	Vertex * vertexBuffer = m_vertexBufferData.data();
	unsigned int vertexCount = 0;
	GLuint * elementBuffer = m_elementBufferData.data();
	for (const BoxMesh & b : m_boxes)
		b.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
}


void BoxObject::create(QOpenGLShaderProgram * shaderProgramm, QOpenGLFunctions * fn) {
	// create and bind Vertex Array Object
	m_vao.create();
	m_vao.bind();

	// create and bind vertex buffer
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_vertexBufferData.size()*sizeof(Vertex);
	qDebug() << "BoxObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
	m_vbo.allocate(m_vertexBufferData.data(), vertexMemSize);

	// create and bind element buffer
	m_ebo.create();
	m_ebo.bind();
	m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
	qDebug() << "BoxObject - ElementBuffer size =" << elementMemSize/1024.0 << "kByte";
	m_ebo.allocate(m_elementBufferData.data(), elementMemSize);

	// set shader attributes
	// tell shader program we have two data arrays to be used as input to the shaders

	// index 0 = position
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
	// index 1 = color
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, r), 3, sizeof(Vertex));
	// index 2 = texture
	shaderProgramm->enableAttributeArray(2); // array with index/id 1
	shaderProgramm->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, texi), 2, sizeof(Vertex));
//	fn->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//							  reinterpret_cast<const void *>(qintptr(offsetof(Vertex, texi)) ) );

	// index 3 = textureID
	shaderProgramm->enableAttributeArray(3); // array with index/id 1
	shaderProgramm->setAttributeBuffer(3, GL_FLOAT, offsetof(Vertex, texID), 1, sizeof(Vertex));
	// shaderProgramm->setAttributeBuffer(3, GL_INT, offsetof(Vertex, texID), 1, sizeof(Vertex));
//	fn->glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(Vertex),
//							  reinterpret_cast<const void *>(qintptr(offsetof(Vertex, texID)) ) );

	// Release (unbind) all
	m_vao.release();
	m_vbo.release();
	m_ebo.release();
}


void BoxObject::destroy() {
	m_vao.destroy();
	m_vbo.destroy();
	m_ebo.destroy();
}


void BoxObject::render() {
	// set the geometry ("position" and "color" arrays)
	m_vao.bind();

	// now draw the cube by drawing individual triangles
	// - GL_TRIANGLES - draw individual triangles via elements
	glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
	// release vertices again
	m_vao.release();
}
