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

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

OpenGLWindow::OpenGLWindow(QWindow *parent) :
	QWindow(parent),
	m_context(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
}


void OpenGLWindow::renderLater() {
	// Schedule an UpdateRequest event in the event loop
	// that will be send with the next VSync.
	requestUpdate(); // call public slot requestUpdate()
}


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
	renderNow(); // simply redirect call to renderNow()
}


void OpenGLWindow::renderNow() {
	if (!isExposed())
		return;

	bool needsInitialize = false;

	// initialize on first call
	if (m_context == nullptr) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}

	m_context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initialize(); // user code
	}

	render(); // user code

	m_context->swapBuffers(this);
}

