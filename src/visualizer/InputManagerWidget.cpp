/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/InputManagerWidget.cpp
*/

#include <QLabel>
#include <QVBoxLayout>

#include "visualizer/InputManagerWidget.h"

namespace Visualizer {

InputManagerWidget::InputManagerWidget(InputManager *inputManager) : m_inputManager(inputManager) {
	setFeatures(QDockWidget::DockWidgetMovable);
	
	setTitleBarWidget(new QLabel(tr("Input Manager")));
	
	QWidget *stackWidget = new QWidget();
	QVBoxLayout *stackLayout = new QVBoxLayout();
	
	m_listView = new QListView();
	
	m_listView->setModel(inputManager);
	
	stackLayout->addWidget(m_listView);
	
	stackWidget->setLayout(stackLayout);
	
	setWidget(stackWidget);
	setVisible(true);
}

InputManagerWidget::~InputManagerWidget() {
	
}

} // namespace Visualizer
