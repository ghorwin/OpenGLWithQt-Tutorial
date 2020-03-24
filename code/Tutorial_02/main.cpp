/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "RectangleWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char **argv) {
	QGuiApplication app(argc, argv);

	// Set OpenGL Version information
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);

	RectangleWindow window;
	// Note: The format must be set before show() is called.
	window.setFormat(format);
	window.resize(640, 480);
	window.show();

	return app.exec();
}
