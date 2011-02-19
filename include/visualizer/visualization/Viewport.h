/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/visualization/Viewport.h

*/

#include <QMdiSubWindow>

#ifndef AesalonVisualizer_Visualization_Viewport_H
#define AesalonVisualizer_Visualization_Viewport_H

namespace Visualizer {
namespace Visualization {

class Viewport : public QMdiSubWindow { Q_OBJECT
public:
	Viewport();
	~Viewport();
};

} // namespace Visualization
} // namespace Visualizer

#endif
