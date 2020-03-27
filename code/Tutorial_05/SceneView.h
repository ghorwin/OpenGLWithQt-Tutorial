#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QMatrix4x4>

#include "OpenGLWindow.h"
#include "ShaderProgram.h"
#include "KeyboardMouseHandler.h"
#include "GridObject.h"
#include "Camera.h"
#include "BoxObject.h"
#include "RectMesh.h"

/*! The class SceneView extends the primitive OpenGLWindow
	by adding keyboard/mouse event handling, and rendering of different
	objects (that encapsulate shader programs and buffer object).
*/
class SceneView : public OpenGLWindow {
public:
	SceneView();
	virtual ~SceneView() override;

	/*! All shader programs used in the scene. */
	QList<ShaderProgram>	m_shaderPrograms;

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

	// Functions to handle key press and mouse press events, all the work is done in class KeyboardMouseHandler
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	void exposeEvent(QExposeEvent *event) override;

private:
	/*! Tests, if any relevant input was received and registers a state change. */
	void checkInput();

	/*! This function is called first thing in the paintGL() routine and
		processes input received so far and updates camera position.
	*/
	void processInput();

	/*! Compines camera matrix and project matrix to form the world2view matrix and also
		marks the scene as dirty (needs repaint).
	*/
	void updateWorld2ViewMatrix();

	/*! This flag marks the scene/camera/project matrix as invalid and will cause a render update
		in next call to paintGL(). If this flag is false, paintGL() does nothing and thus also does
		not waste resources.
	*/
	bool						m_needRepaint;
	/*! If set to true, an input event was received, which will be evaluated at next repaint. */
	bool						m_inputEventReceived;

	// Fix for Windows OS, to avoid "2 vsync delay" lag
	QRegion						m_cachedRegion;

	/*! The input handler, that encapsulates the event handling code. */
	KeyboardMouseHandler		m_keyboardMouseHandler;

	/*! The projection matrix, updated whenever the viewport geometry changes (in resizeGL() ). */
	QMatrix4x4					m_projection;
	Transform3D					m_transform;	// world transformation matrix
	Camera						m_camera;		// Camera position, orientation and lens data
	QMatrix4x4					m_worldToView;

	BoxObject					m_boxObject;
	GridObject					m_gridObject;
};

#endif // SCENEVIEW_H
