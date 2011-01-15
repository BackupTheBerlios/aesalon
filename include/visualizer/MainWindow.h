/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/MainWindow.h

*/

#include <QMainWindow>

#ifndef AesalonVisualizer_MainWindow_H
#define AesalonVisualizer_MainWindow_H

namespace Visualizer {

class MainWindow : public QMainWindow { Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();
private:
	void setupMenus();
};

} // namespace Visualizer

#endif
