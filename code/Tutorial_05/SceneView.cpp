#include "SceneView.h"

#include <QExposeEvent>

SceneView::SceneView() :
	m_needRepaint(false)
{
	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
	ShaderProgram blocks(":/shaders/world2view.vert",":/shaders/simple.frag");
	blocks.m_uniformNames.append("worldToView");
	m_shaderPrograms.append( blocks );

	// Shaderprogram #1 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/simple.frag");
	grid.m_uniformNames.append("worldToView");
	grid.m_uniformNames.append("backgroundColor");
	m_shaderPrograms.append( grid );
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
	// the projection matrix need to be updated only for window size changes
	m_projection.setToIdentity();
	// create projection matrix, i.e. camera lens
	m_projection.perspective(
				/* vertical angle */ 45.0f,
				/* aspect ratio */   width / float(height),
				/* near */           0.1f,
				/* fa r*/            1000.0f
		);
	// Mind: to not use 0.0 for near plane, otherwise depth buffering and depth testing won't work!

	updateWorld2ViewMatrix();
}


void SceneView::paintGL() {
	// only paint if we need to
	if (!m_needRepaint)
		return;

	qDebug() << "[SceneView::paintGL]";

}


void SceneView::exposeEvent(QExposeEvent *event) {
	if (event->region() != m_cachedRegion) {
		// the check against the cached region is needed for Windows OS.
		// There, when the window is moved around, the function onFrameSwapped() is called
		// due to the frameSwapped() signal and also exposeEvent() is sent, because
		// the window manager tells us that the entire screen is invalidated.
		// This results in two QOpenGLWindow::update() calls and a noticable 2 vsync delay.
		m_cachedRegion = event->region();
		qDebug() << "SceneView::exposeEvent" << m_cachedRegion;

		m_needRepaint = true;
		OpenGLWindow::exposeEvent(event); // this will trigger a repaint
	}
	else {
		event->ignore();
	}
}


void SceneView::updateWorld2ViewMatrix() {

}
