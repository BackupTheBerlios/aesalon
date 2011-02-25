/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/SymbolResolver.cpp
*/

#include "monitor/SymbolResolver.h"
#include "storage/RTree.h"
#include <monitor/ElfParser.h>

namespace Monitor {

SymbolResolver::SymbolResolver() {
	m_rtree = new RTree();
}

SymbolResolver::~SymbolResolver() {
	delete m_rtree;
}

void SymbolResolver::parse(const std::string &filename) {
	Message(Debug, "Asked to parse \"" << filename << "\"");
	class Processor : public ElfParser::Processor {
	private:
		RTree *m_rtree;
	public:
		Processor(RTree *rtree) : m_rtree(rtree) {}
		virtual ~Processor() {}
		
		virtual void process(const char *symbolName, uint64_t symbolAddress, uint64_t symbolSize) {
			RTree::Bound b;
			b.setRange(RTree::Range(symbolAddress, symbolAddress + symbolSize), 0);
			Message(Debug, "Processing symbol \"" << symbolName << "\"");
			m_rtree->insert(b, "");
		}
	};
	
	Processor p(m_rtree);
	
	ElfParser ep;
	ep.parse(filename, &p);
}

} // namespace Monitor
