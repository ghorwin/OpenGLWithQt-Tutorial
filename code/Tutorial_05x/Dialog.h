#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class OpenGLRenderWindow;


/*! A primitive dialog, only here to test how to embed a native OpenGLWidget
	into a widget-based UI.
*/
class Dialog : public QDialog {
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = nullptr);
	~Dialog();

private slots:
	void on_checkBoxAnimate_toggled(bool checked);

	void on_pushButton_clicked();

private:
	Ui::Dialog		*ui;

	/*! The native window with the direct onscreen OpenGL rendering.
		We could also use a QOpenGLWidget, but this would mean we are rendering
		always to an off-screen framebuffer first, before copying it to the screen - bad for
		high-resolution stuff. However, we should to a performence test at some point.
	*/
	OpenGLRenderWindow	*openGLWindow;
};

#endif // DIALOG_H
