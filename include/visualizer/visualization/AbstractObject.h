/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/visualization/AbstractObject.h

*/

#ifndef AesalonVisualizer_Visualization_AbstractObject_H
#define AesalonVisualizer_Visualization_AbstractObject_H

namespace Visualizer {
namespace Visualization {

class AbstractObject {
public:
	virtual ~AbstractObject() {}
	
	virtual void paint() = 0;
};

} // namespace Visualization
} // namespace Visualizer

#endif
