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
	typedef Storage::RTree<double, int, 3, 4, 2> RTree;
	
	RTree rt;
	
	RTree::Bound b;
	
	b.setRange(RTree::Range(38.3, 46.9), 0);
	b.setRange(RTree::Range(77.7, 79.2), 1);
	b.setRange(RTree::Range(79.3, 82.8), 2);
	rt.insert(b, 1);
	b.setRange(RTree::Range(38.6, 47.8), 0);
	b.setRange(RTree::Range(64.9, 67), 1);
	b.setRange(RTree::Range(36.2, 38.9), 2);
	rt.insert(b, 2);
	b.setRange(RTree::Range(69, 74.9), 0);
	b.setRange(RTree::Range(76.3, 78.9), 1);
	b.setRange(RTree::Range(54, 56.6), 2);
	rt.insert(b, 3);
	b.setRange(RTree::Range(17.2, 20.8), 0);
	b.setRange(RTree::Range(21.1, 27.9), 1);
	b.setRange(RTree::Range(56.7, 59.6), 2);
	rt.insert(b, 4);
	b.setRange(RTree::Range(78.2, 81.2), 0);
	b.setRange(RTree::Range(86.2, 88.5), 1);
	b.setRange(RTree::Range(6.7, 10.2), 2);
	rt.insert(b, 5);
	b.setRange(RTree::Range(92.9, 93.1), 0);
	b.setRange(RTree::Range(2.2, 8), 1);
	b.setRange(RTree::Range(6.9, 13.6), 2);
	rt.insert(b, 6);
	b.setRange(RTree::Range(39.3, 44.9), 0);
	b.setRange(RTree::Range(1.1, 5.3), 1);
	b.setRange(RTree::Range(22.9, 30.2), 2);
	rt.insert(b, 7);
	b.setRange(RTree::Range(42.1, 44), 0);
	b.setRange(RTree::Range(78.4, 82.1), 1);
	b.setRange(RTree::Range(19.8, 22.2), 2);
	rt.insert(b, 8);
	b.setRange(RTree::Range(31.5, 38.5), 0);
	b.setRange(RTree::Range(41.3, 43.9), 1);
	b.setRange(RTree::Range(9.1, 17.1), 2);
	rt.insert(b, 9);
	b.setRange(RTree::Range(95.6, 102.9), 0);
	b.setRange(RTree::Range(86.2, 93.2), 1);
	b.setRange(RTree::Range(99.6, 107.7), 2);
	rt.insert(b, 10);

	
	class Processor : public RTree::SearchProcessor {
	public:
		virtual bool process(const RTree::Bound &bound, int value) {
			Message(Debug, "Found value " << value);
			return true;
		}
	};
	
	Processor p;
	
	RTree::Range searchRange[] = {
		RTree::Range(0.0, 40.0),
		RTree::Range(0.0, 45.0),
		RTree::Range(0.0, 40.0)
	};
	
	b.setRange(RTree::Range( 0.0, 45.0), 0);
	b.setRange(RTree::Range( 0.0, 45.0), 1);
	b.setRange(RTree::Range( 0.0, 45.0), 2);
	
	rt.search(b, &p);
	
	return 0;
}
#endif
