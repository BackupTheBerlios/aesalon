/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/SymbolResolver.cpp
*/

#include <cstring>

#include "monitor/SymbolResolver.h"
#include "storage/RTree.h"
#include "monitor/ElfParser.h"
#include "monitor/Coordinator.h"

namespace Monitor {

SymbolResolver::SymbolResolver() {
	m_rtree = new RTree();
}

SymbolResolver::~SymbolResolver() {
	delete m_rtree;
}

void SymbolResolver::parse(const std::string &filename, uint64_t offset) {
	Message(Debug, "Asked to parse \"" << filename << "\"");
	class Processor : public ElfParser::Processor {
	private:
		RTree *m_rtree;
		uint64_t m_offset;
		Storage::Mempool *m_mempool;
	public:
		Processor(RTree *rtree, uint64_t offset) : m_rtree(rtree), m_offset(offset),
			m_mempool(Coordinator::instance()->mempool()) {}
		virtual ~Processor() {}
		
		virtual void process(const char *symbolName, uint64_t symbolAddress, uint64_t symbolSize) {
			symbolAddress += m_offset;
			RTree::Bound b;
			b.setRange(RTree::Range(symbolAddress, symbolAddress + symbolSize), 0);
			
			char *name = static_cast<char *>(m_mempool->request(std::strlen(symbolName)));
			strcpy(name, symbolName);
			m_rtree->insert(b, name);
		}
	};
	
	Processor p(m_rtree, offset);
	
	ElfParser ep;
	ep.parse(filename, &p);
}

} // namespace Monitor
