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
#include "util/MessageSystem.h"
#include "visualizer/InputManagerWidget.h"
#include "visualizer/LogInput.h"
#include "visualizer/SettingsDialog.h"

namespace Visualizer {

RootWindow::RootWindow() {
	setWindowTitle(tr("Aesalon Visualizer"));
	setWindowIcon(QIcon(":/icon.png"));
	
	m_mdiArea = new QMdiArea();
	//m_mdiArea->setViewMode(QMdiArea::TabbedView);
	setCentralWidget(m_mdiArea);
	
	m_inputManager = new InputManager();
	
	m_inputManager->addInput(new LogInput("/home/ethereal/projects/aesalon/output-3.alog", m_inputManager->artisanManager()));
	
	InputManagerWidget *imw = new InputManagerWidget(m_inputManager);
	
	imw->setParent(this);
	imw->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, imw);
	
	createAboutBox();
	
	QMenu *aesalonMenu = new QMenu(tr("&Aesalon"));
	aesalonMenu->addAction(tr("&Settings"), this, SLOT(showSettingsDialog()));
	aesalonMenu->addAction(tr("&Close"), this, SLOT(close()));
	aesalonMenu->addAction(tr("&Quit"), qApp, SLOT(closeAllWindows()));
	menuBar()->addMenu(aesalonMenu);
	
	QMenu *helpMenu = new QMenu(tr("&Help"));
	helpMenu->addAction(tr("&About . . ."), m_aboutAesalon, SLOT(show()));
	helpMenu->addAction(tr("About &Qt . . ."), qApp, SLOT(aboutQt()));
	menuBar()->addMenu(helpMenu);
}

RootWindow::~RootWindow() {
	
}

void RootWindow::addSubwindow(Artisan::Viewport *viewport) {
	QMdiSubWindow *msw = m_mdiArea->addSubWindow(viewport);
	msw->setAttribute(Qt::WA_DeleteOnClose);
	msw->setWindowTitle(viewport->windowTitle());
	msw->show();
}

void RootWindow::showSettingsDialog() {
	SettingsDialog *dialog = new SettingsDialog();
	dialog->setModal(true);
	dialog->show();
}

void RootWindow::createAboutBox() {
	m_aboutAesalon = new QDialog();
	m_aboutAesalon->setModal(true);
	
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
