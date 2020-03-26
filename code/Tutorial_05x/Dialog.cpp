#include "Dialog.h"
#include "ui_Dialog.h"

#include <QSurfaceFormat>
#include <QVBoxLayout>
#include <QColorDialog>

#include "OpenGLRenderWindow.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint | Qt::Window | Qt::WindowCloseButtonHint),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);

	// Set OpenGL Version information
	// Note: This format must be set before show() is called.
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSamples(4); // 4 sample antialiasing
	format.setVersion(3,3);
	format.setDepthBufferSize( 24 );

	openGLWindow = new OpenGLRenderWindow;
	openGLWindow->setFormat(format);

	// we now create a widget container, place it into the layout and insert the OpenGLWindow in it.

	QWidget *container = QWidget::createWindowContainer(openGLWindow);
	container->setMinimumSize(QSize(640,480));
	container->setFocusPolicy(Qt::TabFocus);

	QVBoxLayout * lay = new QVBoxLayout;

	ui->widgetViewContainer->setLayout(lay);
//	ui->widgetViewContainer->setWindowFlags(Qt::FramelessWindowHint);
	lay->addWidget(container);
}


Dialog::~Dialog() {
	delete ui;
	delete openGLWindow;
}


void Dialog::on_checkBoxAnimate_toggled(bool checked) {
	openGLWindow->m_animate = checked;
	openGLWindow->update();
}


void Dialog::on_pushButton_clicked() {
	QColorDialog dlg(this);
	QColor c = dlg.getColor();
	if (c.isValid()) {
		openGLWindow->m_boxObject.m_rectangles[0].m_color = QVector3D(c.redF(), c.greenF(), c.blueF());
		openGLWindow->m_sceneChanged = true;
		openGLWindow->update();
	}
}
