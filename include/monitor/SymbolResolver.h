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

namespace Storage {
template<typename Key, typename Value, int dimensions, int Maximum, int Minimum, typename FloatKey> class RTree;
} // namespace Storage

namespace Monitor {

class SymbolResolver {
protected:
	typedef Storage::RTree<uint64_t, const char *, 1, 64, 32, double> RTree;
private:
	RTree *m_rtree;
public:
	SymbolResolver();
	~SymbolResolver();
	
	void parse(const std::string &filename);
};

} // namespace Monitor

#endif
