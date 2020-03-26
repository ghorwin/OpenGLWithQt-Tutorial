#include "SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>

#define SHADER(x) m_shaderPrograms[x].shaderProgram()

SceneView::SceneView() :
	m_needRepaint(false)
{
	// create camera
	m_camera.translate(0,2,0);
	m_camera.rotate(-30, m_camera.right());

	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_W);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_A);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_S);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_D);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Q);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_E);

	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
	ShaderProgram blocks(":/shaders/world2view.vert",":/shaders/simple.frag");
	blocks.m_uniformNames.append("worldToView");
	m_shaderPrograms.append( blocks );

	// Shaderprogram #1 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/simple.frag");
	grid.m_uniformNames.append("worldToView");
	grid.m_uniformNames.append("gridColor");
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

	// initialize drawable objects
//	m_gridObject.create(m_shaderPrograms[1].shaderProgram());
	m_boxObject.create(SHADER(0));

	// tell OpenGL to show only faces whose normal vector points towards us
	glEnable(GL_CULL_FACE);

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

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

	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	// set the background color = clear color
	QVector3D backgroundColor(0.1f, 0.1f, 0.3f);
	glClearColor(backgroundColor.x(), backgroundColor.y(), backgroundColor.z(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// clear the background color
	// render using our shader
	SHADER(0)->bind();
	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	SHADER(0)->setUniformValue(u_worldToView, m_worldToView);
	{
		// set the geometry ("position" and "color" arrays)
		m_boxObject.m_vao.bind();

		// now draw the cube by drawing individual triangles
		// - GL_TRIANGLES - draw individual triangles
		glDrawArrays(GL_TRIANGLES, 0, m_boxObject.m_NVertexes);
		// release vertices again
		m_boxObject.m_vao.release();
	}
	SHADER(0)->release();

#ifdef DRAW_GRID
	QOpenGLShaderProgram * gridProgram = m_shaderPrograms[1].shaderProgram();
	if (!gridProgram->bind())
		qDebug() << gridProgram->log();

	// assign the projection matrix to the parameter identified by 'u_worldToView' in the shader code
	gridProgram->setUniformValue(m_shaderPrograms[1].m_uniformIDs[0], m_worldToView);
	QVector3D gridColor(0.3f, 0.6f, 0.3f);
	gridProgram->setUniformValue(m_shaderPrograms[1].m_uniformIDs[1], gridColor);
	gridProgram->setUniformValue(m_shaderPrograms[1].m_uniformIDs[2], backgroundColor);
	{
		// bind vertex array object
		m_gridObject.m_vao.bind();

		// now draw the grid lines
		glDrawArrays(GL_LINES, 0, m_gridObject.m_NVertexes);
		// release vertices again
		m_gridObject.m_vao.release();
	}
	gridProgram->release();
	m_needRepaint = false;
#endif
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
	qDebug() << "SceneView::updateWorld2ViewMatrix";
	// projectedPoint = camera2project * world2camera
	Transform3D transform;
	transform.translate(0.0f, 0.0f, -5.0f);

	m_worldToView = m_projection * m_camera.toMatrix() * transform.toMatrix();
	qDebug() << "m_projection = " << m_projection;
	qDebug() << "m_camera = " << m_camera.toMatrix();
	qDebug() << "m_transform = " << transform.toMatrix();
	qDebug() << "world2View = " << m_worldToView;
	m_needRepaint = true;
}
