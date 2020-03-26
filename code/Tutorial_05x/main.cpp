#include <QApplication>

#include "Dialog.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	Dialog dlg;
	dlg.show();

	return app.exec();
}
