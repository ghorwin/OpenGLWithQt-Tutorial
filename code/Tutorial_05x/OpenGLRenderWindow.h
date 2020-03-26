#ifndef OpenGLRenderWindow_H
#define OpenGLRenderWindow_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "Transform3d.h"
#include "Camera.h"
#include "KeyboardMouseHandler.h"
#include "RectMesh.h"

class QExposeEvent;

class QOpenGLShaderProgram;

/*! This is the OpenGL Render Window, which also holds the scene graph.
*/
class OpenGLRenderWindow :
		public QOpenGLWindow,
		protected QOpenGLFunctions
{
	Q_OBJECT
public:
	OpenGLRenderWindow();
	~OpenGLRenderWindow() Q_DECL_OVERRIDE;

	// OpenGL Events
	void initializeGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

	/*! If set to true, the cube is spinning, otherwise it will be static. */
	bool m_animate;
	std::vector<RectMesh>		m_rectangles;
	bool						m_sceneChanged;

private slots:
	/*! Called when vsync has just happened and we *may* redraw the scene. */
	void onFrameSwapped();

protected:
	/*! Re-implemented to update scene upon exposing of window. */
	void exposeEvent(QExposeEvent *ev) Q_DECL_OVERRIDE;

	// Functions to handle key press and mouse press events
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
	void printVersionInformation();
	void updateScene();

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
	int							u_worldToView;	// cache for variable index of parameter 'worldToCamera' declared in vertex shader

	/*! Holds the compiled shader program for grid lines. */
	QOpenGLShaderProgram		*m_gridProgram;
	int							u_gridWorldToView;	// cache for variable index of parameter 'worldToCamera' declared in vertex shader
	int							u_gridColor;			// cache for variable index of parameter 'gridColor' declared in vertex shader


	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates and color buffers. */
	QOpenGLVertexArrayObject	m_gridVao;

	QMatrix4x4					m_projection;	// updated in resizeGL() function
	Transform3D					m_transform;	// world transformation matrix,
	Camera						m_camera;		// Camera position, orientation and lens data

	KeyboardMouseHandler		m_inputHandler;

	// Fix for Windows OS, to avoid "2 vsync delay" lag
	QRegion						m_cachedRegion;

	bool						m_updateNeeded; // cached flag that is set whenever we need to redraw OpenGL
};

#endif // OpenGLRenderWindow_H
