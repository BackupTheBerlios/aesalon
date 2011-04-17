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
	class Processor : public ElfParser::Processor {
	private:
		RTree *m_rtree;
		uint64_t m_offset;
	public:
		Processor(RTree *rtree, uint64_t offset) : m_rtree(rtree), m_offset(offset) {}
		virtual ~Processor() {}
		
		virtual void process(const char *symbolName, uint64_t symbolAddress, uint64_t symbolSize) {
			symbolAddress += m_offset;
			
			char *name = new char[std::strlen(symbolName)];
			strcpy(name, symbolName);
			m_rtree->insert(RTree::BoundType(symbolAddress, symbolAddress + symbolSize), name);
		}
	};
	
	Processor p(m_rtree, offset);
	
	ElfParser ep;
	ep.parse(filename, &p);
}

} // namespace Monitor
