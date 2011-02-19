/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/storage/MemoryPoolItem.h

*/

#ifndef AesalonVisualizer_Storage_MemoryPoolItem_H
#define AesalonVisualizer_Storage_MemoryPoolItem_H

namespace Visualizer {
namespace Storage {

class MemoryPoolItem {
public:
	virtual ~MemoryPoolItem() {}
};

} // namespace Storage
} // namespace Visualizer

#endif
