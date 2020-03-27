/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TestDialog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "SceneView.h"

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
	format.setSamples(4);	// enable multisampling (antialiasing)

	m_sceneView = new SceneView;
	m_sceneView->setFormat(format);

	// *** create window container widget

	QWidget *container = QWidget::createWindowContainer(m_sceneView);
	container->setMinimumSize(QSize(1000,800));

	// *** create the layout and insert widget container

	QVBoxLayout * vlay = new QVBoxLayout;
	vlay->setMargin(0);
	vlay->setSpacing(0);
	vlay->addWidget(container);

	// now create some buttons at the bottom

	QHBoxLayout * hlay = new QHBoxLayout;
	hlay->setMargin(0);

	QPushButton * closeBtn = new QPushButton(tr("Close"), this);
	connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

	hlay->addStretch();
	hlay->addWidget(closeBtn);

	vlay->addLayout(hlay);

	setLayout(vlay);

	resize(1200,900);
}

