/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file visualizer/src/visualization/RenderWidget.cpp

*/

#include "visualization/RenderWidget.h"

namespace Visualizer {
namespace Visualization {

RenderWidget::RenderWidget() {
	m_view = new QGraphicsView();
}

RenderWidget::~RenderWidget() {

}

} // namespace Visualization
} // namespace Visualizer
