/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

Source code is based on Qt Example OpenGLWindow, but has been simplified a lot.

************************************************************************************/

#include "OpenGLWindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

OpenGLWindow::OpenGLWindow(QWindow *parent) :
	QWindow(parent),
	m_context(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
}


void OpenGLWindow::initOpenGL() {
	bool needsInitialize = false;

	// protect against calling this function twice (accidentally)
	if (m_context == nullptr) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}

	m_context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initializeGL(); // call user code
	}
}


void OpenGLWindow::renderLater() {
	// Schedule an UpdateRequest event in the event loop
	// that will be send with the next VSync.
	requestUpdate(); // call public slot requestUpdate()
}


void OpenGLWindow::renderNow() {
	if (!isExposed())
		return;

	// initialize on first call
	if (m_context == nullptr)
		initOpenGL();

	m_context->makeCurrent(this);

	paintGL(); // call user code

	m_context->swapBuffers(this);
}


// *** protected functions ***

bool OpenGLWindow::event(QEvent *event) {
	switch (event->type()) {
	case QEvent::UpdateRequest:
		renderNow();
		return true;
	default:
		return QWindow::event(event);
	}
}


void OpenGLWindow::exposeEvent(QExposeEvent * /*event*/) {
	renderNow(); // update right now

	// Note: if were just to request an update on next sync, i.e. by
	//       calling renderLater() (or requestUpdate()) we get
	//       white glitches when enlarging the window. Since we don't want that,
	//       we simply render right away so that the new window size
	//       is already reflected by the adjusted viewport we render into.
}


void OpenGLWindow::resizeEvent(QResizeEvent * event) {
	QWindow::resizeEvent(event);

	// initialize on first call
	if (m_context == nullptr)
		initOpenGL();

	resizeGL(width(), height());
}


