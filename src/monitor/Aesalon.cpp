/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/

#include "monitor/Coordinator.h"

#include "storage/RTree.h"

#if 0
int main(int argc, char *argv[]) {
	
	
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	return coordinator.returnValue();
}
#endif


int main(int argc, char *argv[]) {
	typedef Storage::RTree<double, int, 1, 8, 2> RTree;
	
	RTree rt;
	
	RTree::Range range[] = {
		RTree::Range(-2.0, 2.0)
	};
	RTree::Bound b(range);
	rt.insert(b, 0);
	
	class Processor : public RTree::SearchProcessor {
	public:
		virtual bool process(const RTree::Bound &bound, int value) {
			Message(Debug, "Found value " << value);
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
