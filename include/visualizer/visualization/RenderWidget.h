/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/visualization/RenderWidget.h

*/

#ifndef AesalonVisualizer_Visualization_RenderWidget_H
#define AesalonVisualizer_Visualization_RenderWidget_H

#include <QObject>
#include <QWidget>

namespace Visualizer {
namespace Visualization {

class RenderWidget : public QWidget { Q_OBJECT
public:
	RenderWidget();
	virtual ~RenderWidget();
private:
	
};

} // namespace Visualization
} // namespace Visualizer

#endif
