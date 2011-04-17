/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/
#include <stdlib.h>
#include <iostream>

#include "monitor/Coordinator.h"
#include "config/GlobalVault.h"

#include "storage/RTree.h"

#if 1
int main(int argc, char *argv[]) {
	Config::GlobalVault gv;
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	return coordinator.returnValue();
}

#else
int main(int argc, char *argv[]) {
	typedef Storage::RTree<double, int, 1, 2, 4> RTree;
	
	RTree rt;
	
	rt.insert(RTree::BoundType(77.7, 79.2), 1);
	rt.insert(RTree::BoundType(64.9, 67), 2);
	rt.insert(RTree::BoundType(76.3, 78.9), 3);
	rt.insert(RTree::BoundType(21.1, 27.9), 4);
	rt.insert(RTree::BoundType(86.2, 88.5), 5);
	rt.insert(RTree::BoundType(2.2, 8), 6);
	
	class Visitor : public RTree::SearchVisitorType {
	public:
		virtual ~Visitor() {}
		
		virtual void visit(const RTree::BoundType &bound, const int &data) {
			Message(Debug, "Found item with data " << data);
		}
	};
	
	Visitor visitor;
	rt.search(RTree::BoundType(0.0, 22.0), visitor);
	
	return 0;
}
#endif
