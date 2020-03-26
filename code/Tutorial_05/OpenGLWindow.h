/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

Source code is based on Qt Example OpenGLWindow, but has been simplified a lot.

************************************************************************************/

#ifndef OpenGLWindow_H
#define OpenGLWindow_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QOpenGLContext;
class QOpenGLContext;
QT_END_NAMESPACE

/*! The OpenGLWindow is very similar to QOpenGLWindow, yet a little more light-weight.
	Also, the functions initializeGL() and paintGL() are protected, as they are in
	the QOpenGLWidget. Thus, you can easily switch to an QOpenGLWidget class later on,
	if you need it.
*/
class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit OpenGLWindow(QWindow *parent = nullptr);

	/*! Explicit function to initialize the window. Can be called prior to
		showing the window during application init.
	*/
	void initOpenGL();

public slots:
	/*! Redirects to slot requestUpdate(), which registers an UpdateRequest event in the event loop
		to be issued with next VSync.
	*/
	void renderLater();

	/*! Directly repaints the view right now (this function is called from event() and exposeEvent(). */
	void renderNow();

protected:
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;
	void resizeEvent(QResizeEvent *) override;

	/*! Called on first show of the window. Re-implement with your own
		OpenGL initialization code.
	*/
	virtual void initializeGL() = 0;

	/*! Called whenever the view port changes (window geometry). Re-implement
		in your own code, for example to update the projection matrix.
		This function is called from resizeEvent() and thus before paintGL().
		\param width Width of window in pixels as returned from width()
		\param height Height of window in pixels as returned from height()
	*/
	virtual void resizeGL(int width, int height) {}

	/*! Called just after the OpenGL context was made current. Re-implement in
		derived classes to do the actual painting.
	*/
	virtual void paintGL() = 0;


	QOpenGLContext *m_context;
};

#endif // OpenGLWindow_H
