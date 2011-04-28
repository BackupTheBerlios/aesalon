/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/SettingsModel.cpp
*/

#include "visualizer/SettingsModel.h"
#include "util/MessageSystem.h"

namespace Visualizer {

SettingsModel::SettingsModel() {
	Config::GlobalVault::instance()->match("*", m_settingsVector);
}

SettingsModel::~SettingsModel() {
	
}

int SettingsModel::columnCount(const QModelIndex &parent) const {
	return 2;
}

int SettingsModel::rowCount(const QModelIndex &parent) const {
	Message(Debug, "rowCount called. Return value: " << m_settingsVector.size() + 1);
	return m_settingsVector.size() + 1;
}

QVariant SettingsModel::data(const QModelIndex &index, int role) const {
	if(role == Qt::DisplayRole) {
		if(index.row() == m_settingsVector.size()) {
			return QVariant("<new item>");
		}
		else if(index.column() == 0) {
			return QString::fromStdString(m_settingsVector[index.row()].first);
		}
		else if(index.column() == 1) {
			return QString::fromStdString(m_settingsVector[index.row()].second);
		}
	}
	return QVariant();
}

QVariant SettingsModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if(role == Qt::DisplayRole) {
		if(orientation == Qt::Horizontal) {
			if(section == 0) return "Name";
			else if(section == 1) return "Value";
		}
	}
	return QVariant();
}

Qt::ItemFlags SettingsModel::flags(const QModelIndex &index) const {
	if((index.row() == m_settingsVector.size() && index.column() == 0) || (index.column() == 1 && index.row() != m_settingsVector.size())) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}
	else {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
}

bool SettingsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if(role != Qt::EditRole) return false;
	if(index.row() == m_settingsVector.size()) {
		m_settingsVector.push_back(Config::Vault::KeyPair(value.toString().toStdString(), ""));
		Config::GlobalVault::instance()->set(value.toString().toStdString(), "");
		emit dataChanged(index, index);
		beginInsertRows(index, index.row()+1, index.row()+1);
		endInsertRows();
	}
	else if(index.column() == 1) {
		if(index.row() > m_settingsVector.size()) return false;
		m_settingsVector[index.row()].second = value.toString().toStdString();
		emit dataChanged(index, index);
	}
	return false;
}

} // namespace Visualizer
