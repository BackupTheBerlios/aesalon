/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer//RootWindow.h

*/

#ifndef AesalonVisualizer_RootWindow_H
#define AesalonVisualizer_RootWindow_H

#include <QMainWindow>

namespace Visualizer {

class RootWindow : public QMainWindow { Q_OBJECT
private:
	
public:
	RootWindow();
	virtual ~RootWindow();
private:
	void initialSetup();
private slots:
	void newRootWindow();
};

} // namespace Visualizer

#endif
