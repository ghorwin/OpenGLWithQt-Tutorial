#include "SceneView.h"

SceneView::SceneView() :
	m_needRepaint(false)
{
}


SceneView::~SceneView() {
	m_context->makeCurrent(this);

	for (ShaderProgram & p : m_shaderPrograms)
		p.destroy();
}


void SceneView::initializeGL() {

	// we process all individual shader programs first
	for (ShaderProgram & p : m_shaderPrograms)
		p.create();

}


void SceneView::resizeGL(int width, int height) {

}


void SceneView::paintGL() {
	// only paint if we need to
	if (!m_needRepaint)
		return;

}


void SceneView::exposeEvent(QExposeEvent *event) {
	if (ev->region() != m_cachedRegion) {
		// the check against the cached region is needed for Windows OS.
		// There, when the window is moved around, the function onFrameSwapped() is called
		// due to the frameSwapped() signal and also exposeEvent() is sent, because
		// the window manager tells us that the entire screen is invalidated.
		// This results in two QOpenGLWindow::update() calls and a noticable 2 vsync delay.
		m_cachedRegion = ev->region();
		qDebug() << "SceneView::exposeEvent" << m_cachedRegion;

		m_updateNeeded = true;
		QOpenGLWindow::exposeEvent(ev);
	}
	else {
		ev->ignore();
	}
}
