/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/storage/MemoryPool.h

*/

#ifndef AesalonVisualizer_Storage_MemoryPool_H
#define AesalonVisualizer_Storage_MemoryPool_H

#include <list>

namespace Visualizer {
namespace Storage {

class MemoryPool {
private:
	typedef std::list<quint8 *> PoolList;
	PoolList m_poolList;
public:
	MemoryPool();
	~MemoryPool();
	
	template<typename Type>
	Type *allocate();
};

} // namespace Storage
} // namespace Visualizer

#endif
