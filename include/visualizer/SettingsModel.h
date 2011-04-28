/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/SettingsModel.h
*/

#ifndef AesalonVisualizer_SettingsModel_H
#define AesalonVisualizer_SettingsModel_H

#include <QAbstractListModel>

#include "config/GlobalVault.h"

namespace Visualizer {

class SettingsModel : public QAbstractTableModel {
private:
	std::vector<Config::GlobalVault::KeyPair> m_settingsVector;
public:
	SettingsModel();
	virtual ~SettingsModel();
	
	virtual int columnCount(const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
};

} // namespace Visualizer

#endif
