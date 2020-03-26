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


	// *** create scene

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
	m_sceneView->m_shaderPrograms.append( ShaderProgram(":/shaders/world2view.vert",":/shaders/simple.frag") );
	// Shaderprogram #1 : grid (painting grid lines)
	m_sceneView->m_shaderPrograms.append( ShaderProgram(":/shaders/grid.vert",":/shaders/grid.frag") );

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

	qsrand(428); // for now use fixed seed so that we get the same colors every time
}

