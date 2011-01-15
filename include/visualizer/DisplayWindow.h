/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer//DisplayWindow.h

*/

#ifndef AesalonVisualizer_DisplayWindow_H
#define AesalonVisualizer_DisplayWindow_H

#include <QObject>

namespace Visualizer {

class DisplayWindow : public QObject { Q_OBJECT
public:
	DisplayWindow();
	virtual ~DisplayWindow();
};

} // namespace Visualizer

#endif
