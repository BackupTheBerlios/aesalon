/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/
#include <stdlib.h>
#include <iostream>
#include <time.h>

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
	typedef Storage::RTree<double, int, 1, 2, 6> RTree;
	
	RTree rt;
	
	srand(1);
	
	struct timespec start, end;
	
	clock_gettime(CLOCK_REALTIME, &start);
	
	for(int i = 0; i < 1000000; i ++) {
		double d = rand();
		rt.insert(RTree::BoundType(d, d + 0.1 + (double)rand()), i);
	}
	
	clock_gettime(CLOCK_REALTIME, &end);
	
	uint64_t elapsed = (end.tv_sec*1000000000+end.tv_nsec)-(start.tv_sec*1000000000+start.tv_nsec);
	Message(Log, "Elapsed time for 1,000,000 insertions: " << elapsed << "ns");
	
	return 0;
}
#endif
