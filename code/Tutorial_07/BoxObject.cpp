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
#include <QElapsedTimer>

#include "PickObject.h"

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

	const int BoxGenCount = 10000;
	const int GridDim = 100; // must be an int, or you have to use a cast below

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


void BoxObject::create(QOpenGLShaderProgram * shaderProgramm) {
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


void BoxObject::pick(const QVector3D & p1, const QVector3D & d, PickObject & po) const {
	// now process all box objects
	for (unsigned int i=0; i<m_boxes.size(); ++i) {
		const BoxMesh & bm = m_boxes[i];
		for (unsigned int j=0; j<6; ++j) {
			float z;
			// is intersection point closes to viewer than previous intersection points?
			if (bm.intersects(j, p1, d, z)) {
//				qDebug() << QString("Plane %1 of box %2 intersects line at z = %3").arg(j).arg(i).arg(z);
				if (z > po.m_z) {
					po.m_z = z;
					po.m_objectId = i;
					po.m_faceId = j;
				}
			}
		}
	}
}


void BoxObject::highlight(unsigned int boxId, unsigned int faceId) {
	// we change the color of all vertexes of the selected box to lightgray
	// and the vertex colors of the selected plane/face to light blue

	std::vector<QColor> faceCols(6);
	for (unsigned int i=0; i<6; ++i) {
		if (i == faceId)
			faceCols[i] = QColor("#b40808");
		else
			faceCols[i] = QColor("#f3f3f3");
	}
	m_boxes[boxId].setFaceColors(faceCols);

	// advance the pointers and vertex numbers to the respected box position/numbering
	Vertex * vertexBuffer = m_vertexBufferData.data() + boxId*6*4; // 6 planes, with 4 vertexes each
	unsigned int vertexCount = boxId*6*4;
	GLuint * elementBuffer = m_elementBufferData.data() + boxId*6*6; // 6 planes, with 2 triangles with 3 indexes each
	// then we update the respective portion of the vertexbuffer memory
	m_boxes[boxId].copy2Buffer(vertexBuffer, elementBuffer, vertexCount);

	QElapsedTimer t;
	t.start();
	// and now update the entire vertex buffer
	m_vbo.bind();
	// only update the modified portion of the data
	m_vbo.write(boxId*6*4*sizeof(Vertex), m_vertexBufferData.data() + boxId*6*4, 6*4*sizeof(Vertex));
	// alternatively use the call below, which (re-) copies the entire buffer, which can be slow
	// m_vbo.allocate(m_vertexBufferData.data(), m_vertexBufferData.size()*sizeof(Vertex));
	m_vbo.release();
	qDebug() << t.elapsed();
}
