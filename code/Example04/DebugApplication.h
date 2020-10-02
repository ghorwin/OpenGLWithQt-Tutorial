#ifndef DEBUGAPPLICATION_H
#define DEBUGAPPLICATION_H

#include <QApplication>
#include <iostream>

#include "OpenGLException.h"

class DebugApplication : public QApplication {
public:
	DebugApplication( int & argc, char ** argv) :  QApplication(argc, argv), m_aboutToTerminate(false)
	{}

	virtual bool notify(QObject *recv, QEvent *e) override {
		try {
			return QApplication::notify( recv, e );
		}
		catch (OpenGLException &ex) {
			ex.writeMsgStackToStream(std::cerr);
			m_aboutToTerminate = true;
			QApplication::exit(1);
		}
		return false;
	}

	// Flag to check for program abort
	bool m_aboutToTerminate;
};

#endif // DEBUGAPPLICATION_H
