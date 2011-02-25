/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/RootWindow.h
*/

#ifndef AesalonVisualizer_RootWindow_H
#define AesalonVisualizer_RootWindow_H

#include <QMainWindow>

namespace Visualizer {

class RootWindow : public QMainWindow { Q_OBJECT
public:
	RootWindow();
	~RootWindow();
};

} // namespace Visualizer

#endif
