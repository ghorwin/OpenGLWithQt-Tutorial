#include "TextObject.h"

#include <QOpenGLTexture>
#include <QFontMetrics>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QOpenGLShaderProgram>

#include "ShaderProgram.h"
#include "PlaneMesh.h"

#define TEXTURE_ID 0

TextObject::TextObject() :
	m_vbo(QOpenGLBuffer::VertexBuffer), // actually the default, so default constructor would have been enough
	m_ebo(QOpenGLBuffer::IndexBuffer) // make this an Index Buffer
{
}


void TextObject::create(ShaderProgram & shaderProgram) {

	// first generate the image holding the individual texts

	int fontSize = 22;
	int textWidth=0;
	int textHeight=0;

	QFont f;
	f.setPointSize(fontSize);

	QFontMetrics fm(f);
	textHeight = fm.lineSpacing();
	for (const TextData & t : m_texts) {
		QRect textRect = fm.boundingRect(t.m_text);
		textWidth = qMax(textWidth, textRect.width());
		textHeight = qMax(textHeight, textRect.height());
	}

	QColor textColor(255,255,255);

	int imgHeight = textHeight*m_texts.size();

	// our texture size shall be limited to 640 px, that will be enough for quite some text
	textWidth = qMin(640, textWidth);

	textWidth = 196;
	imgHeight = 196;
	QImage textimg(textWidth, imgHeight, QImage::Format_RGBA8888);
	{
		QPainter painter(&textimg);
		painter.fillRect(0, 0, textWidth, imgHeight, QColor(55,55,255,0));
		painter.setBrush(textColor);
		painter.setPen(textColor);
		painter.setFont(f);
		int height = textHeight;
		for (TextData & t : m_texts) {
			painter.drawText(0, height, t.m_text);

			// store coordinates of text bounding rect on texture
			t.m_texX1 = 0; // currently always 0
			t.m_texX2 = fm.boundingRect(t.m_text).width()/float(textWidth);
			t.m_texY1 = height+1;
			t.m_texY2 = height - textHeight+1;
			// invert and normalize j coords
			t.m_texY1 = t.m_texY1/float(imgHeight);
			t.m_texY2 = t.m_texY2/float(imgHeight);

			// now update the vertexes to have the correct aspect ratio
			QVector3D a = t.m_b-t.m_a;
			float lenA = a.length();
			a /= lenA; // normalize A
			QVector3D b = t.m_d-t.m_a;
			float lenB = b.length();


			float aspect = fm.boundingRect(t.m_text).width()/float(textHeight);
			lenA = aspect*lenB;
			// compute new point b
			t.m_b = t.m_a + a*lenA;

			height += textHeight;
		}
	}
//	bool success = textimg.save("/home/ghorwin/bla.png");

	shaderProgram.shaderProgram()->bind();
	// create texture
	m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	m_texture->create();
	// texture attributes
	m_texture->setMinificationFilter(QOpenGLTexture::NearestMipMapLinear);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_texture->setWrapMode(QOpenGLTexture::ClampToBorder);
	m_texture->setData(textimg); // allocate() will be called internally
	qDebug()<< "Texture mipmap levels: " << m_texture->mipLevels();
	// tell shader to associate texture uniform 'text01' with a texture index
	// Basically, this means that the texture uniform named 'text01' in the fragmentation shader,
	// whose uniformIndex was stored in location m_shaderPrograms[0].m_uniformIDs[1], will
	// now be associated with an OpenGL texture bound to index TEXTURE_ID (=0 here)
	// later we bind our texted with index 0
	shaderProgram.shaderProgram()->setUniformValue(shaderProgram.m_uniformIDs[1], TEXTURE_ID);

	// resize storage arrays
	m_vertexBufferData.resize(m_texts.size()*PlaneMesh::VertexCount);
	m_elementBufferData.resize(m_texts.size()*PlaneMesh::IndexCount);

	// for each text we have already setup a plane in m_texts
	// update the buffers
	VertexTex * vertexBuffer = m_vertexBufferData.data();
	unsigned int vertexCount = 0;
	GLuint * elementBuffer = m_elementBufferData.data();
	for (const TextData & t : m_texts) {
		// create a plane mesh and copy the data to buffer
		PlaneMesh m(t.m_a, t.m_b, t.m_d);
		m.m_texi1 = t.m_texX1;
		m.m_texi2 = t.m_texX2;
		m.m_texj1 = t.m_texY1;
		m.m_texj2 = t.m_texY2;
		m.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
	}


	// create and bind Vertex Array Object
	m_vao.create();
	m_vao.bind();

	// create and bind vertex buffer
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_vertexBufferData.size()*sizeof(VertexTex);
	m_vbo.allocate(m_vertexBufferData.data(), vertexMemSize);

	// create and bind element buffer
	m_ebo.create();
	m_ebo.bind();
	m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
	m_ebo.allocate(m_elementBufferData.data(), elementMemSize);

	// set shader attributes
	// tell shader program we have two data arrays to be used as input to the shaders

	// index 0 = position
	shaderProgram.shaderProgram()->enableAttributeArray(0); // array with index/id 0
	shaderProgram.shaderProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(VertexTex));
	// index 1 = texture coordinates
	shaderProgram.shaderProgram()->enableAttributeArray(1); // array with index/id 1
	shaderProgram.shaderProgram()->setAttributeBuffer(1, GL_FLOAT, offsetof(VertexTex, texi), 2, sizeof(VertexTex));

	// Release (unbind) all
	m_vao.release();
	m_vbo.release();
	m_ebo.release();

	shaderProgram.shaderProgram()->release();
}


void TextObject::destroy() {
	m_vao.destroy();
	m_vbo.destroy();
	m_ebo.destroy();
	delete m_texture;
}


void TextObject::render() {
	// then render all planes
	// bind the texture as texture with index 0
	m_texture->bind(TEXTURE_ID);

	m_vao.bind();

	// now draw the cube by drawing individual triangles
	// - GL_TRIANGLES - draw individual triangles via elements
	glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);
	// release vertices again
	m_vao.release();
}


void TextObject::addText(const QString & text, const QVector3D & a, const QVector3D & b, const QVector3D & d) {
	m_texts.push_back(TextData(text, a, b, d));
}
