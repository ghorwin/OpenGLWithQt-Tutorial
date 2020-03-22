/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TriangleWindow.h"

#include <QDebug>

TriangleWindow::TriangleWindow() :
	m_program(nullptr)
{
}


TriangleWindow::~TriangleWindow() {
	// resource cleanup
	m_vao.destroy();
	m_vertexBufferObject.destroy();
	delete m_program;
}


void TriangleWindow::initialize() {
	// this function is called once, when the window is first shown, i.e. when
	// the the window content is first rendered

	// build and compile our shader program
	// ------------------------------------

	m_program = new QOpenGLShaderProgram();

	// read the shader programs from the resource
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/pass_through.vert"))
		qDebug() << "Vertex shader errors:\n" << m_program->log();

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uniform_color.frag"))
		qDebug() << "Fragment shader errors:\n" << m_program->log();

	if (!m_program->link())
		qDebug() << "Shader linker errors:\n" << m_program->log();


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// Initialize the Vertex Array Object (VAO)

	m_vao.create(); // create underlying OpenGL object
	m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so we can write attributes to it

	// create a new buffer for the vertices
	m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer); // VBO
	m_vertexBufferObject.create(); // create underlying OpenGL object
	m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw); // must be called before allocate

	m_vertexBufferObject.bind(); // set it active in the context, so that we can write to it
	// int bufSize = sizeof(vertices) = 9 * sizeof(float) = 9*4 = 36 bytes
	m_vertexBufferObject.allocate(vertices, sizeof(vertices) ); // copy data into buffer

	// Enable attribute array at layout location 0
	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
	// This maps the data we have set in the buffer to the "position" attribute.
	// 0 - offset - means the "position" data starts at the begin of the memory array
	// 3 - size of each vertex (=vec3) - means that each position-tuple has the size of 3 floats (those are the 3 coordinates,

	// Release (unbind) all
	m_vertexBufferObject.release();
	m_vao.release();
}


void TriangleWindow::render() {
	// this function is called for every frame to be rendered on screen
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// use our shader program
	m_program->bind();
	// bind the vertex buffer
	m_vao.bind();
	// now draw the triangles:
	// - GL_TRIANGLES - draw individual triangles
	// - 0 index of first triangle to draw
	// - 3 number of vertices to process
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
