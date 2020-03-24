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
std::vector<float> vertexBufferData(2*4*3); // 2 attributes: position and color, 4 vertices, 3 floats each

TriangleWindow::TriangleWindow() :
	m_program(nullptr)
{
}


TriangleWindow::~TriangleWindow() {
	// resource cleanup

	// since we release resources related to an OpenGL context,
	// we make this context current before cleaning up our resources
	makeCurrent();

	// resource cleanup
	m_vao.destroy();
	m_vertexBufferObject.destroy();
	delete m_program;
}


void TriangleWindow::initializeGL() {
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
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	QColor vertexColors [] = {
		QColor("#f6a509"),
		QColor("#cb2dde"),
		QColor("#0eeed1"),
		QColor("#068918"),
	};

	// create a new buffer for the vertices and colors, interleaved storage
	m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vertexBufferObject.create();
	m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vertexBufferObject.bind();

#if 0
	// create new data buffer - the following memory copy stuff should
	// be placed in some convenience class in later tutorials
	// copy data in interleaved mode with pattern v0c0|v1c1|v2c2|v3c3
	float * buf = vertexBufferData.data();
	for (int v=0; v<4; ++v, buf += 6) {
		// coordinates
		buf[0] = vertices[3*v];
		buf[1] = vertices[3*v+1];
		buf[2] = vertices[3*v+2];
		// colors
		buf[3] = vertexColors[v].redF();
		buf[4] = vertexColors[v].greenF();
		buf[5] = vertexColors[v].blueF();
	}
#endif
	// now copy buffer data over
	m_vertexBufferObject.allocate(vertices, sizeof(vertices) );

	// create a new buffer for the indexes
	m_indexBufferObject = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer); // Mind: use 'IndexBuffer' here
	m_indexBufferObject.create();
	m_indexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_indexBufferObject.bind();
	m_indexBufferObject.allocate(indices, sizeof(indices) );

	// Initialize the Vertex Array Object (VAO) to record and remember subsequent attribute assocations with
	// generated vertex buffer(s)
	m_vao.create(); // create underlying OpenGL object
	m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so it monitors attribute assignments

	// layout location 0 - vec3 with coordinates
	int stride = 6*sizeof(float); // one vertex data set contains of 3+3 floats = stride
	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);
	// layout location 1 - vec3 with colors
//	m_program->enableAttributeArray(1);
//	int colorOffset = 3*sizeof(float);
//	m_program->setAttributeBuffer(1, GL_FLOAT, colorOffset, 3, stride);

	// Release (unbind) all
	m_vertexBufferObject.release();
//	m_indexBufferObject.release();
	m_vao.release();
}


void TriangleWindow::paintGL() {

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// use our shader program
	m_program->bind();
	// bind the vertex array object, which in turn binds the vertex buffer object and
	// sets the attribute buffer in the OpenGL context
	m_vao.bind();
	m_indexBufferObject.bind();
	// now draw the two triangles via index drawing
	// - GL_TRIANGLES - draw individual triangles via elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	// finally release VAO again (not really necessary, just for completeness)
	m_vao.release();
}
