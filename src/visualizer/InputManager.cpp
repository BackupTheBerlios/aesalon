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
	m_artisanManager = new ArtisanManager();
}

InputManager::~InputManager() {
	delete m_artisanManager;
}

void InputManager::addInput(DataInput *input) {
	Message(Debug, "Adding input . . .");
	m_inputList.push_back(input);
	emit inputAdded(input);
}

int InputManager::columnCount(const QModelIndex &parent) const {
	return 1;
}

QVariant InputManager::data(const QModelIndex &index, int role) const {
	int row = index.row();
	int col = index.column();
	if(role == Qt::DisplayRole) {
		if(col == 0 && row < m_inputList.length()) {
			return m_inputList[row]->title();
		}
	}
	else if(role == Qt::CheckStateRole) {
		if(col == 0 && row < m_inputList.length()) {
			if(m_inputList[row]->isEnabled()) return Qt::Checked;
			else return Qt::Unchecked;
		}
	}
	return QVariant();
}

int InputManager::rowCount(const QModelIndex &parent) const {
	return m_inputList.length();
}

QVariant InputManager::headerData(int section, Qt::Orientation orientation, int role) const {
	return QVariant();
}

Qt::ItemFlags InputManager::flags(const QModelIndex &index) const {
	if(index.column() == 0) return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool InputManager::setData(const QModelIndex &index, const QVariant &value, int role) {
	if(role == Qt::CheckStateRole) {
		int row = index.row();
		if(row < 0 || row > m_inputList.length()) return false;
		if(value.toBool()) {
			m_inputList[row]->enable();
		}
		else {
			m_inputList[row]->disable();
		}
		return true;
	}
	return false;
}

} // namespace Visualizer
