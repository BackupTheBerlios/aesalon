/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/
#include <stdlib.h>

#include "monitor/Coordinator.h"

#include "storage/RTree.h"

#if 0
int main(int argc, char *argv[]) {
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	return coordinator.returnValue();
}


#else
int main(int argc, char *argv[]) {
	typedef Storage::RTree<double, int, 3, 4, 2> RTree;
	
	RTree rt;
	
	RTree::Bound b;
	
	for(int i = 0; i < 14; i ++) {
		double start = (rand()%1000)/10.0;
		double size = (rand()%100)/10.0;
		b.setRange(RTree::Range(start, start+size), 0);
		start = (rand()%1000)/10.0;
		size = (rand()%100)/10.0;
		b.setRange(RTree::Range(start, start+size), 1);
		start = (rand()%1000)/10.0;
		size = (rand()%100)/10.0;
		b.setRange(RTree::Range(start, start+size), 2);
		rt.insert(b, i);
	}
	
	class Processor : public RTree::SearchProcessor {
	public:
		virtual bool process(const RTree::Bound &bound, int value) {
			Message(Debug, "Found value " << value);
			return true;
		}
	};
	
	Processor p;
	
	RTree::Range searchRange[] = {
		RTree::Range(-3000.0, 3000.0),
		RTree::Range(-3000.0, 3000.0),
		RTree::Range(-3000.0, 3000.0)
	};
	
	RTree::Bound sb(searchRange);
	
	rt.search(searchRange, &p);
	
	return 0;
}
#endif
