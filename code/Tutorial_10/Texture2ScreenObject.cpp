/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "Texture2ScreenObject.h"

#include <QOpenGLShaderProgram>
#include <vector>


void Texture2ScreenObject::create(QOpenGLShaderProgram * shaderProgramm) {
	// Create Vertex Array Object
	m_vao.create();		// create Vertex Array Object
	m_vao.bind();		// and bind it

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// Create Vertex Buffer Object
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo.allocate(quadVertices, sizeof(quadVertices));

	// layout(location = 0) = vec2 position
	shaderProgramm->enableAttributeArray(0); // array with index/id 0
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT,
								  0 /* position/vertex offset */,
								  2 /* two floats per position = vec2 */,
								  4*sizeof(float) /* stride = 4 floats */);
	shaderProgramm->enableAttributeArray(1); // array with index/id 1
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT,
								  2*sizeof(float) /* texture index offset */,
								  2 /* two floats per position = vec2 */,
								  4*sizeof(float) /* stride = 4 floats */);
	m_vao.release();
	m_vbo.release();
}


void Texture2ScreenObject::destroy() {
	m_vao.destroy();
	m_vbo.destroy();
}


void Texture2ScreenObject::render() {
	m_vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_vao.release();
}
