#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QMatrix4x4>

#include "OpenGLWindow.h"
#include "ShaderProgram.h"
#include "KeyboardMouseHandler.h"
#include "OpenGLGridObject.h"
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
	std::vector<RectMesh>		m_rectangles;

	// Shader Information
	int							u_worldToView;  // cache for variable index of parameter 'u_worldToCamera' declared in vertex shader

	// Fix for Windows OS, to avoid "2 vsync delay" lag
	QRegion						m_cachedRegion;

	/*! The input handler, that encapsulates the event handling code. */
	KeyboardMouseHandler		m_keyboardMouseHandler;

	/*! The projection matrix, updated whenever the viewport geometry changes (in resizeGL() ). */
	QMatrix4x4					m_projection;

	Camera						m_camera;
	QMatrix4x4					m_worldToView;

	// stuff to draw

	std::vector<float>			m_vertexBufferData;
	std::vector<GLuint>			m_elementBufferData;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_vao;
	/*! Holds position and colors in a single buffer. */
	QOpenGLBuffer				m_vertexBuffer;
	/*! Holds elements. */
	QOpenGLBuffer				m_elementBuffer;

	/*! Holds position of grid lines. */
	QOpenGLBuffer				m_gridVertexBuffer;
	std::vector<float>			m_gridVertexBufferData;

	/*! Holds the compiled shader programs. */
	QOpenGLShaderProgram		*m_program;

	/*! Holds the compiled shader program for grid lines. */
	QOpenGLShaderProgram		*m_gridProgram;
	int							u_gridWorldToView;	// cache for variable index of parameter 'worldToCamera' declared in vertex shader
	int							u_gridColor;			// cache for variable index of parameter 'gridColor' declared in vertex shader


	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_gridVao;





//	BoxObject					m_boxObject;
//	OpenGLGridObject			m_gridObject;
};

#endif // SCENEVIEW_H