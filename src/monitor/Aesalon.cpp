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
	typedef Storage::RTree<double, int, 1, 5, 2> RTree;
	
	RTree rt;
	
	/*rt.insert(RTree::Bound(77.7, 79.2), 1);
	rt.insert(RTree::Bound(64.9, 67), 2);
	rt.insert(RTree::Bound(76.3, 78.9), 3);
	rt.insert(RTree::Bound(21.1, 27.9), 4);
	rt.insert(RTree::Bound(86.2, 88.5), 5);*/
	/*rt.insert(RTree::Bound(2.2, 8), 6);
	rt.insert(RTree::Bound(1.1, 5.3), 7);
	rt.insert(RTree::Bound(78.4, 82.1), 8);
	rt.insert(RTree::Bound(41.3, 43.9), 9);
	rt.insert(RTree::Bound(86.2, 93.2), 10);*/
	
	rt.insert(RTree::Bound(2.2, 8), 6);
	rt.insert(RTree::Bound(1.1, 5.3), 7);
	
	class Processor : public RTree::Callback {
	public:
		virtual bool handle(const RTree::Bound &/*bound*/, int &value) {
			Message(Debug, "****\t\tFound value " << value);
			return true;
		}
	};
	
	Processor p;
	
	rt.search(RTree::Bound(0.0, 8.0), &p);
	rt.search(RTree::Bound(0.0, 16.0), &p);
	
	return 0;
}
#endif
