/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/
#include <stdlib.h>
#include <iostream>

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
	typedef Storage::RTree<double, int, 1, 5, 2> RTree;
	
	RTree rt;
	
	RTree::Bound b;
	
	b.setRange(RTree::Range(77.7, 79.2), 0);
	rt.insert(b, 1);
	b.setRange(RTree::Range(64.9, 67), 0);
	rt.insert(b, 2);
	b.setRange(RTree::Range(76.3, 78.9), 0);
	rt.insert(b, 3);
	b.setRange(RTree::Range(21.1, 27.9), 0);
	rt.insert(b, 4);
	b.setRange(RTree::Range(86.2, 88.5), 0);
	rt.insert(b, 5);
	b.setRange(RTree::Range(2.2, 8), 0);
	rt.insert(b, 6);
	b.setRange(RTree::Range(1.1, 5.3), 0);
	rt.insert(b, 7);
	b.setRange(RTree::Range(78.4, 82.1), 0);
	rt.insert(b, 8);
	b.setRange(RTree::Range(41.3, 43.9), 0);
	rt.insert(b, 9);
	b.setRange(RTree::Range(86.2, 93.2), 0);
	rt.insert(b, 10);
	
	b.setRange(RTree::Range(21.1, 27.9), 0);
	rt.remove(b, 4);
	b.setRange(RTree::Range(2.2, 8), 0);
	rt.remove(b, 6);
	b.setRange(RTree::Range(1.1, 5.3), 0);
	rt.remove(b, 7);
	
	class Processor : public RTree::SearchProcessor {
	public:
		virtual bool process(const RTree::Bound &bound, int value) {
			Message(Debug, "****\t\tFound value " << value);
			return true;
		}
	};
	
	Processor p;
	
	b.setRange(RTree::Range(0.0, 8.0), 0);
	
	rt.search(b, &p);
	
	return 0;
}
#endif
