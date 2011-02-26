/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/PIDAllocator.h
*/

#ifndef AesalonVisualizer_PIDAllocator_H
#define AesalonVisualizer_PIDAllocator_H

#include <stdint.h>

namespace Visualizer {

class PIDAllocator {
private:
	static uint32_t m_nextID;
public:
	static uint32_t nextID() { return ++m_nextID; }
};

} // namespace Visualizer

#endif
