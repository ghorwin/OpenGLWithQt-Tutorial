#include "TestDialog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "RectangleWindow.h"

TestDialog::TestDialog() :
#ifdef Q_OS_WIN
	QDialog(nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
#else
	QDialog(nullptr, Qt::Window)
#endif
{
	// *** create OpenGL window

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);

	m_rectangleWindow = new RectangleWindow;
	m_rectangleWindow->setFormat(format);

	// *** create window container widget

	QWidget *container = QWidget::createWindowContainer(m_rectangleWindow);
	container->setMinimumSize(QSize(600,400));

	// *** create the layout and insert widget container

	QVBoxLayout * vlay = new QVBoxLayout;
	vlay->setMargin(0);
	vlay->setSpacing(0);
	vlay->addWidget(container);

	// now create some buttons at the bottom

	QHBoxLayout * hlay = new QHBoxLayout;
	hlay->setMargin(0);

	QPushButton * changeColorBtn = new QPushButton(tr("Change colors"), this);
	connect(changeColorBtn, &QPushButton::clicked, this, &TestDialog::onChangeColors);

	QPushButton * changeColorAnimationBtn = new QPushButton(tr("Animate colors"), this);
	connect(changeColorAnimationBtn, &QPushButton::clicked, this, &TestDialog::onAnimateColors);

	QPushButton * closeBtn = new QPushButton(tr("Close"), this);
	connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

	hlay->addWidget(changeColorBtn);
	hlay->addWidget(changeColorAnimationBtn);
	hlay->addStretch();
	hlay->addWidget(closeBtn);

	vlay->addLayout(hlay);

	setLayout(vlay);

	qsrand(428); // for now use fixed seed so that we get the same colors every time
}


void TestDialog::onChangeColors() {
	// randomize the colors and change them in the OpenGL window

	for (unsigned int i=0; i<4; ++i) {
		m_rectangleWindow->m_vertexColors[i].setRgbF(rand()*1.0/RAND_MAX, rand()*1.0/RAND_MAX, rand()*1.0/RAND_MAX);
	}

	// now update the scene -> this will also request an update
	m_rectangleWindow->updateScene();
}


void TestDialog::onAnimateColors() {
	// randomize the colors and change them in the OpenGL window

	std::vector<QColor> newCols(4);
	for (unsigned int i=0; i<4; ++i) {
		newCols[i].setRgbF(rand()*1.0/RAND_MAX, rand()*1.0/RAND_MAX, rand()*1.0/RAND_MAX);
	}

	// now update the scene -> this will also request an update
	m_rectangleWindow->animateColorsTo(newCols);
}
