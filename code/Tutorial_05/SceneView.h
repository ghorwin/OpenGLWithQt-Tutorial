#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QMatrix4x4>

#include "OpenGLWindow.h"
#include "ShaderProgram.h"
#include "KeyboardMouseHandler.h"

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
	void keyPressEvent(QKeyEvent *event) override			{ m_keyboardMouseHandler.keyPressEvent(event); }
	void keyReleaseEvent(QKeyEvent *event) override			{ m_keyboardMouseHandler.keyReleaseEvent(event); }
	void mousePressEvent(QMouseEvent *event) override		{ m_keyboardMouseHandler.mousePressEvent(event); }
	void mouseReleaseEvent(QMouseEvent *event) override		{ m_keyboardMouseHandler.mouseReleaseEvent(event); }
	void mouseMoveEvent(QMouseEvent *event) override		{ m_keyboardMouseHandler.mouseMoveEvent(event); }
	void wheelEvent(QWheelEvent *event) override			{ m_keyboardMouseHandler.wheelEvent(event); }

	void exposeEvent(QExposeEvent *event) override;

private:
	/*! Compines camera matrix and project matrix to form the world2view matrix and also
		marks the scene as dirty (needs repaint).
	*/
	void updateWorld2ViewMatrix();

	/*! This flag marks the scene/camera/project matrix as invalid and will cause a render update
		in next call to paintGL(). If this flag is false, paintGL() does nothing and thus also does
		not waste resources.
	*/
	bool m_needRepaint;

	// Fix for Windows OS, to avoid "2 vsync delay" lag
	QRegion						m_cachedRegion;

	/*! The input handler, that encapsulates the event handling code. */
	KeyboardMouseHandler		m_keyboardMouseHandler;

	/*! The projection matrix, updated whenever the viewport geometry changes (in resizeGL() ). */
	QMatrix4x4					m_projection;
};

#endif // SCENEVIEW_H
