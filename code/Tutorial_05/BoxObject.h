#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
class QOpenGLFunctions;
QT_END_NAMESPACE

#include "RectMesh.h"

class BoxObject {
public:
	BoxObject();

	/*! The function is called during OpenGL initialization, where the OpenGL context is current. */
	void create(QOpenGLShaderProgram * shaderProgramm);

	void render(QOpenGLFunctions * f);

	std::vector<RectMesh>		m_rectangles;

	std::vector<float>			m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_vao;
	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vertexDataBuffer;

	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vertexBuffer;
	/*! Holds elements. */
	QOpenGLBuffer				m_elementBuffer;
};

#endif // BOXOBJECT_H
