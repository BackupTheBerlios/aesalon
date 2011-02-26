/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/RootWindow.cpp
*/

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPushButton>
#include <QMdiSubWindow>

#include "visualizer/RootWindow.h"
#include "visualizer/InputManagerWidget.h"
#include "util/MessageSystem.h"

namespace Visualizer {

RootWindow::RootWindow() {
	setWindowTitle(tr("Aesalon Visualizer"));
	setWindowIcon(QIcon(":/icon.png"));
	
	m_mdiArea = new QMdiArea();
	//m_mdiArea->setViewMode(QMdiArea::TabbedView);
	setCentralWidget(m_mdiArea);
	
	m_inputManager = new InputManager();
	
	createAboutBox();
	
	QMenu *aesalonMenu = new QMenu(tr("&Aesalon"));
	aesalonMenu->addAction(tr("&Close"), this, SLOT(close()));
	aesalonMenu->addAction(tr("&Quit"), qApp, SLOT(closeAllWindows()));
	menuBar()->addMenu(aesalonMenu);
	
	QMenu *windowMenu = new QMenu(tr("&Window"));
	windowMenu->addAction(tr("&Create input manager"), this, SLOT(createInputManager()));
	menuBar()->addMenu(windowMenu);
	
	QMenu *helpMenu = new QMenu(tr("&Help"));
	helpMenu->addAction(tr("&About . . ."), m_aboutAesalon, SLOT(show()));
	helpMenu->addAction(tr("About &Qt . . ."), qApp, SLOT(aboutQt()));
	menuBar()->addMenu(helpMenu);
	
}

RootWindow::~RootWindow() {
	
}

void RootWindow::createInputManager() {
	InputManagerWidget *imw = new InputManagerWidget(m_inputManager);
	QMdiSubWindow *msw = m_mdiArea->addSubWindow(imw);
	msw->setAttribute(Qt::WA_DeleteOnClose);
	msw->setWindowTitle(tr("Input manager"));
	msw->show();
}

void RootWindow::createAboutBox() {
	m_aboutAesalon = new QWidget();
	
	QBoxLayout *layout = new QHBoxLayout();
	m_aboutAesalon->setLayout(layout);
	
	QLabel *iconLabel = new QLabel();
	iconLabel->setPixmap(QPixmap(":/icon.png"));
	layout->addWidget(iconLabel);
	
	QVBoxLayout *rightLayout = new QVBoxLayout();
	QLabel *aboutText = new QLabel();
	aboutText->setText(
		"Aesalon: a tool to visualize memory in real-time.<br />"
		"Copyright (C) 2009-2011, Aesalon Development Team. <br />"
		"Aesalon is released under the terms of the GNU GPLv3. "
		"See the included file LICENSE for details."
		"<hr />"
		);
	
	rightLayout->addWidget(aboutText);
	
	QPushButton *okay = new QPushButton(tr("Ok"));
	rightLayout->addWidget(okay);
	connect(okay, SIGNAL(clicked(bool)), m_aboutAesalon, SLOT(hide()));
	
	layout->addLayout(rightLayout);
}

} // namespace Visualizer
