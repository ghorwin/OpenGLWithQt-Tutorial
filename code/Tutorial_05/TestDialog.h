#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

class RectangleWindow;

/*! A simple test dialog for an embedded OpenGL window. */
class TestDialog : public QDialog {
	Q_OBJECT
public:
	TestDialog();

private slots:
	void onChangeColors();
	void onAnimateColors();

private:
	RectangleWindow * m_rectangleWindow;
};

#endif // TESTDIALOG_H
