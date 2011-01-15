/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file visualizer/src/MainWindow.cpp

*/

#include <QMenu>
#include <QMenuBar>
#include <QDebug>

#include "MainWindow.h"

namespace Visualizer {

MainWindow::MainWindow() {
	setupMenus();
}

MainWindow::~MainWindow() {
	
}

void MainWindow::setupMenus() {
	QMenuBar *mb = menuBar();
	
	QAction *action = NULL;
	
	QMenu *aesalonMenu = new QMenu(tr("Aesalon"));
	
	action = aesalonMenu->addAction(tr("Quit"));
	connect(action, SIGNAL(triggered(bool)), SLOT(close()));
	
	mb->addMenu(aesalonMenu);
}

} // namespace Visualizer
