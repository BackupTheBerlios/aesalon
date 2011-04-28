/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/SettingsDialog.cpp
*/

#include <QVBoxLayout>

#include "visualizer/SettingsDialog.h"

namespace Visualizer {

SettingsDialog::SettingsDialog() {
	m_settingsModel = new SettingsModel();
	m_settingsTable = new QTableView();
	m_settingsTable->setModel(m_settingsModel);
	
	QVBoxLayout *mainLayout = new QVBoxLayout();
	
	mainLayout->addWidget(m_settingsTable);
	
	setLayout(mainLayout);
}

SettingsDialog::~SettingsDialog() {
	
}

} // namespace Visualizer