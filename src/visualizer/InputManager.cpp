/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/InputManager.cpp
*/

#include "visualizer/InputManager.h"
#include "util/MessageSystem.h"

namespace Visualizer {

InputManager::InputManager() {
	
}

InputManager::~InputManager() {
	
}

void InputManager::addInput(DataInput *input) {
	Message(Debug, "Adding input . . .");
	m_inputList.push_back(input);
	emit inputAdded(input);
}

} // namespace Visualizer
