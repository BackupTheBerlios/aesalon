/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/storage/Mempool.cpp
*/

#include <sys/mman.h>
#include <errno.h>
#include <cstring>

#include "storage/Mempool.h"
#include "util/MessageSystem.h"

namespace Storage {

Mempool::Mempool() {
	
}

Mempool::~Mempool() {
	
}

void *Mempool::request(uint32_t size) {
	int pools = m_mapVector.size();
	int pool;
	for(pool = 0; pool < pools; pool ++) {
		if(m_useVector[pool] + size <= m_mempoolSize) break;
	}
	
	if(pool == pools) createNew();
	
	m_useVector[pool] += size;
	
	return (m_mapVector[pool] + m_useVector[pool] - size);
}

void Mempool::createNew() {
	void *memory = mmap(NULL, m_mempoolSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if(memory == MAP_FAILED) {
		Message(Fatal, "Could not create memory mapping: " << std::strerror(errno));
	}
	m_mapVector.push_back(static_cast<uint8_t *>(memory));
	m_useVector.push_back(0);
}

} // namespace Storage
