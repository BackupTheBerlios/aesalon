/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/RootWindow.h
*/

#ifndef AesalonVisualizer_RootWindow_H
#define AesalonVisualizer_RootWindow_H

#include <QDialog>
#include <QMainWindow>
#include <QMdiArea>
#include <QWidget>

#include "InputManager.h"

namespace Visualizer {

class RootWindow : public QMainWindow { Q_OBJECT
private:
	QMdiArea *m_mdiArea;
	QDialog *m_aboutAesalon;
	InputManager *m_inputManager;
public:
	RootWindow();
	virtual ~RootWindow();
private slots:
	void addSubwindow(Artisan::Viewport *viewport);
	void showSettingsDialog();
private:
	void createAboutBox();
};

} // namespace Visualizer

#endif
