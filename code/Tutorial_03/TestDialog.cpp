#include "TestDialog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "RectangleWindow.h"

TestDialog::TestDialog() {
	// first create the OpenGL window
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);

	m_rectangleWindow = new RectangleWindow;
	m_rectangleWindow->setFormat(format);

	// we now create a widget container
	QWidget *container = QWidget::createWindowContainer(m_rectangleWindow);
	container->setMinimumSize(QSize(600,400));
	container->setFocusPolicy(Qt::TabFocus);

	// create the layout and insert widget container
	QVBoxLayout * vlay = new QVBoxLayout;
	vlay->addWidget(container);

	// now create some buttons at the bottom

	QHBoxLayout * hlay = new QHBoxLayout;

	QPushButton * changeColorBtn = new QPushButton(tr("Change colors"), this);
	connect(changeColorBtn, &QPushButton::clicked, this, &TestDialog::onChangeColorsClicked);

	QPushButton * closeBtn = new QPushButton(tr("Close"), this);
	connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

	hlay->addWidget(changeColorBtn);
	hlay->addStretch();
	hlay->addWidget(closeBtn);

	vlay->addLayout(hlay);

	setLayout(vlay);
}


void TestDialog::onChangeColorsClicked() {
	// randomize the colors and change them in the OpenGL window

}
