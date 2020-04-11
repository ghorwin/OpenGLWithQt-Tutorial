/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "ShaderProgram.h"

#include <QOpenGLShaderProgram>
#include <QDebug>

#include "OpenGLException.h"

ShaderProgram::ShaderProgram(const QString & vertexShaderFilePath, const QString & fragmentShaderFilePath) :
	m_vertexShaderFilePath(vertexShaderFilePath),
	m_fragmentShaderFilePath(fragmentShaderFilePath),
	m_program(nullptr)
{
}


void ShaderProgram::create() {
	FUNCID(ShaderProgram::create);
	Q_ASSERT(m_program == nullptr);

	// build and compile our shader program
	// ------------------------------------

	m_program = new QOpenGLShaderProgram();

	// read the shader programs from the resource
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertexShaderFilePath))
		throw OpenGLException(QString("Error compiling vertex shader %1:\n%2").arg(m_vertexShaderFilePath).arg(m_program->log()), FUNC_ID);

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, m_fragmentShaderFilePath))
		throw OpenGLException(QString("Error compiling fragment shader %1:\n%2").arg(m_fragmentShaderFilePath).arg(m_program->log()), FUNC_ID);

	if (!m_program->link())
		throw OpenGLException(QString("Shader linker error:\n%2").arg(m_program->log()), FUNC_ID);

	m_uniformIDs.clear();
	for (const QString & uniformName : m_uniformNames) {
		m_uniformIDs.append( m_program->uniformLocation(uniformName));
		if (m_uniformIDs.back() == -1)
			throw OpenGLException(QString("Error retrieving uniform ID for uniform '%1'").arg(uniformName), FUNC_ID);
	}
}


void ShaderProgram::destroy() {
	delete m_program;
	m_program = nullptr;
}
