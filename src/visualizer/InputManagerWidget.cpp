/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/InputManagerWidget.cpp
*/

#include <QGridLayout>
#include <QLabel>

#include "visualizer/InputManagerWidget.h"

namespace Visualizer {

InputManagerWidget::InputManagerWidget(InputManager *inputManager) : m_inputManager(inputManager) {
	QGridLayout *layout = new QGridLayout();
	setLayout(layout);
	layout->addWidget(new QLabel("Test label"));
}

InputManagerWidget::~InputManagerWidget() {

}

} // namespace Visualizer
