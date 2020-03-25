/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TestDialog.h"

#include <QApplication>

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	TestDialog dlg;
	dlg.show();

	return app.exec();
}
