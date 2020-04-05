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
#include <ctime>

#include <QApplication>
#include <QDateTime>

#include "OpenGLException.h"
#include "DebugApplication.h"

void qDebugMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	(void) context;
	QString msgPrefix = "[" + QDateTime::currentDateTime().toString() + "] ";
	switch (type) {
		case QtDebugMsg		: msgPrefix += "Debug:    "; break;
		case QtWarningMsg	: msgPrefix += "Warning:  "; break;
		case QtCriticalMsg	: msgPrefix += "Critical: "; break;
		case QtFatalMsg		: msgPrefix += "Fatal:    "; break;
		case QtInfoMsg		: msgPrefix += "Info:     "; break;
	}
	QStringList lines = msg.split("\n");
	for (const QString & l : lines)
		std::cout << (msgPrefix + l).toStdString() << std::endl;
}


int main(int argc, char **argv) {
	qInstallMessageHandler(qDebugMsgHandler);

	DebugApplication app(argc, argv);

	qsrand(time(nullptr));

	TestDialog dlg;
	dlg.show();
	return app.exec();
}
