/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/visualization/AbstractRangeMatcher.h

*/

#ifndef AesalonVisualizer_Visualization_AbstractRangeMatcher_H
#define AesalonVisualizer_Visualization_AbstractRangeMatcher_H

#include "AbstractObject.h"

namespace Visualizer {
namespace Visualization {

class AbstractRangeMatcher {
public:
	virtual ~AbstractRangeMatcher() {}
	
	virtual bool matches(AbstractObject *object) = 0;
};

} // namespace Visualization
} // namespace Visualizer

#endif
