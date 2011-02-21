/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/

#include "monitor/Coordinator.h"

#include "storage/RTree.h"

#if 1
int main(int argc, char *argv[]) {
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	return coordinator.returnValue();
}


#else
int main(int argc, char *argv[]) {
	typedef Storage::RTree<double, int, 1, 4, 2> RTree;
	
	RTree rt;
	
	RTree::Bound b;
	b.setRange(RTree::Range(-2.0, 2.0), 0);
	rt.insert(b, 0);
	b.setRange(RTree::Range(-1.0, 1.0), 0);
	rt.insert(b, 1);
	b.setRange(RTree::Range(-1.5, 1.0), 0);
	rt.insert(b, 2);
	
	b.setRange(RTree::Range(3.5, 6.0), 0);
	rt.insert(b, 3);
	b.setRange(RTree::Range(-6.0, -3.5), 0);
	rt.insert(b, 4);
	
	class Processor : public RTree::SearchProcessor {
	public:
		virtual bool process(const RTree::Bound &bound, int value) {
			Message(Debug, "Found value " << value);
			return true;
		}
	};
	
	Processor p;
	
	RTree::Range searchRange[] = {
		RTree::Range(-3.0, 3.0)
	};
	
	RTree::Bound sb(searchRange);
	
	rt.search(searchRange, &p);
	
	return 0;
}
#endif
