#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

class SceneView;

/*! A simple test dialog for an embedded OpenGL window. */
class TestDialog : public QDialog {
	Q_OBJECT
public:
	TestDialog();

private:
	// The scene view, that shows our world and allows navigation
	SceneView * m_sceneView;
};

#endif // TESTDIALOG_H
