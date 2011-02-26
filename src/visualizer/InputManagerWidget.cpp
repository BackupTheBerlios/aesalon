/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/InputManagerWidget.cpp
*/

#include <QVBoxLayout>
#include <QPushButton>

#include "visualizer/InputManagerWidget.h"
#include "visualizer/InputCreator.h"

namespace Visualizer {

InputManagerWidget::InputManagerWidget(InputManager *inputManager) : m_inputManager(inputManager) {
	QVBoxLayout *layout = new QVBoxLayout();
	
	m_listWidget = new QListWidget();
	layout->addWidget(m_listWidget);
	
	QPushButton *creatorButton = new QPushButton(tr("&Add input"));
	connect(creatorButton, SIGNAL(clicked()), this, SLOT(showCreator()));
	layout->addWidget(creatorButton);
	
	setLayout(layout);
	
	connect(m_inputManager, SIGNAL(inputAdded(DataInput *)), this, SLOT(inputAdded(DataInput *)));
	
	foreach(DataInput *input, inputManager->inputList()) inputAdded(input);
}

InputManagerWidget::~InputManagerWidget() {
	
}

void InputManagerWidget::showCreator() {
	InputCreator ic(m_inputManager);
	connect(&ic, SIGNAL(inputCreated(DataInput *)), m_inputManager, SLOT(addInput(DataInput *)));
	ic.show();
	ic.exec();
}

void InputManagerWidget::inputAdded(DataInput *input) {
	new QListWidgetItem("Testing", m_listWidget);
}

} // namespace Visualizer
