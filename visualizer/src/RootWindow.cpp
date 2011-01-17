/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file visualizer/src//RootWindow.cpp

*/

#include "RootWindow.h"
#include <QMenuBar>
#include <QStyle>
#include <QDockWidget>
#include <QLabel>

namespace Visualizer {

RootWindow::RootWindow() {
	initialSetup();
}

RootWindow::~RootWindow() {
	
}

void RootWindow::initialSetup() {
	/* Basic geometry . . . */
	setMinimumWidth(600);
	setMinimumHeight(400);
	
	/* Icon . . . */
	setWindowIcon(QIcon(":/data/aesalon-icon-96x96.png"));
	
	/* Menus . . . */
	QMenu *menu = NULL;
	QAction *action = NULL;
	
	menu = menuBar()->addMenu(tr("&Aesalon"));
	action = menu->addAction(
		style()->standardIcon(QStyle::SP_FileDialogNewFolder),
		tr("&New"));
	connect(action, SIGNAL(triggered(bool)), SLOT(newRootWindow()));
	action = menu->addAction(
		style()->standardIcon(QStyle::SP_DialogCloseButton),
		tr("&Close"));
	connect(action, SIGNAL(triggered(bool)), SLOT(close()));
	
	QDockWidget *testWidget = new QDockWidget(tr("Test"), this);
	testWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	testWidget->setWidget(new QLabel("Testing"));
	addDockWidget(Qt::LeftDockWidgetArea, testWidget);
	
	setCentralWidget(new QLabel("main"));
}

void RootWindow::newRootWindow() {
	RootWindow *rw = new RootWindow();
	rw->show();
}

} // namespace Visualizer
