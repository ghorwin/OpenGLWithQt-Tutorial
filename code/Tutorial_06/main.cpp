/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TestDialog.h"

#include <iostream>

#include <QApplication>
#include "OpenGLException.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	qsrand(time(nullptr));

	try {
		TestDialog dlg;
		dlg.show();
	}
	catch (OpenGLException & ex) {
		ex.writeMsgStackToStream(std::cerr);
		return EXIT_FAILURE;
	}

	return app.exec();
}
