#include "TriangleWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char **argv) {
	QGuiApplication app(argc, argv);

	// Set OpenGL Version information
	// Note: This format must be set before show() is called.
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);

	TriangleWindow window;
	window.setFormat(format);
	window.resize(640, 480);
	window.show();

	return app.exec();
}
