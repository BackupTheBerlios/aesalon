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

#if 0
int main(int argc, char *argv[]) {
	Config::GlobalVault gv;
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	return coordinator.returnValue();
}

#else
int main(int argc, char *argv[]) {
	typedef Storage::RTree<double, int, 1, 2, 6> RTree;
	
	RTree rt;
	
	srand(1);
	
	class Visitor : public RTree::SearchVisitorType {
	public:
		virtual ~Visitor() {}
		
		virtual void visit(const RTree::BoundType &bound, const int &data) {
			Message(Debug, "Found item with data " << data);
		}
	};
	
	Visitor visitor;
	
	for(int i = 0; i < 10; i ++) {
		double d = (rand()%1000)/10.0;
		rt.insert(RTree::BoundType(d, d + 0.1 + (rand()%500)/10.0), i);
		rt.search(RTree::BoundType(0.0, 150.0), visitor);
	}
	
	return 0;
}
#endif
