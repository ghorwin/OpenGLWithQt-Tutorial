#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

#include "Vertex.h"

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
class QOpenGLTexture;
QT_END_NAMESPACE

class ShaderProgram;

/*! A text object encapsulates several texts that are drawn using the same textures.
	A text texture can hold several lines of (short text).
	Each text object is defined as a plane object with texture coordinates attached.

	Drawing text requrires a texture shader with transparency yet not lighting.
	Vertex shader takes only vertexes and texture coordinates.
	Fragment shader generates transparent textures.
*/
class TextObject {
public:
	TextObject();
	void create(ShaderProgram & shaderProgram);
	void destroy();

	void render();

	void addText(const QString & text, const QVector3D & a, const QVector3D & b, const QVector3D & d);

	struct TextData {
		TextData() {}
		TextData(const QString & text, const QVector3D & a, const QVector3D & b, const QVector3D & d) :
			m_text(text),
			m_a(a), m_b(b), m_d(d) {}
		QString		m_text;		// the text to render
		QVector3D	m_a;
		QVector3D	m_b;
		QVector3D	m_d;
		float		m_texX1;
		float		m_texY1;
		float		m_texX2;
		float		m_texY2;
	};

	std::vector<TextData>	m_texts;

private:
	unsigned int	m_lineSpacing; // determined in create(), used to set the texture coordinates

	std::vector<VertexTex>		m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and texture infos buffers. */
	QOpenGLVertexArrayObject	m_vao;

	/*! Holds position and texture infos in a single buffer. */
	QOpenGLBuffer				m_vbo;
	/*! Holds elements. */
	QOpenGLBuffer				m_ebo;

	/*! The texture, that holds the text to render. */
	QOpenGLTexture				*m_texture;

};

#endif // TEXTOBJECT_H
