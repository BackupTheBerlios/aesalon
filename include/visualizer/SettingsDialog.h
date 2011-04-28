/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/SettingsDialog.h
*/

#ifndef AesalonVisualizer_SettingsDialog_H
#define AesalonVisualizer_SettingsDialog_H

#include <QDialog>
#include <QTableView>

#include "SettingsModel.h"

namespace Visualizer {

class SettingsDialog : public QDialog {
private:
	QTableView *m_settingsTable;
	SettingsModel *m_settingsModel;
public:
	SettingsDialog();
	virtual ~SettingsDialog();
};

} // namespace Visualizer

#endif
