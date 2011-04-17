/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/SymbolResolver.h
*/

#ifndef AesalonMonitor_SymbolResolver_H
#define AesalonMonitor_SymbolResolver_H

#include <stdint.h>
#include <string>

#include "storage/RTree.h"

namespace Monitor {

class SymbolResolver {
protected:
	typedef Storage::RTree<uint64_t, const char *, 1, 2, 16, double> RTree;
private:
	RTree *m_rtree;
public:
	SymbolResolver();
	~SymbolResolver();
	
	void parse(const std::string &filename, uint64_t offset);
};

} // namespace Monitor

#endif
