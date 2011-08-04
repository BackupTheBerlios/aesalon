/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/storage/RTree.cpp
*/

#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "Config.h"

#include "storage/RTree.h"

namespace Storage {
namespace RTreePrivate {

template<int min, int max>
static void benchmarkTimingsFor() {
	typedef Storage::RTree<double, int, 3, min, max> RTree;
	
	RTree rt;
	
	srand(1);
	
	struct timespec start, end;
	
	Message2(Log, Storage, "Benchmark results for (min,max) = (" << min << "," << max << ")");
	
	clock_gettime(CLOCK_REALTIME, &start);
	
	for(int i = 0; i < 1000000; i ++) {
		double x = rand();
		double y = rand();
		double z = rand();
		rt.insert(typename RTree::BoundType(
			x, x + 0.1 + (double)rand(),
			y, y + 0.1 + (double)rand(),
			z, z + 0.1 + (double)rand()
			), i);
	}
	
	clock_gettime(CLOCK_REALTIME, &end);
	
	uint64_t elapsed = (end.tv_sec*1000000000+end.tv_nsec)-(start.tv_sec*1000000000+start.tv_nsec);
	Message2(Log, Storage, "\tElapsed time for 1,000,000 random insertions: " << elapsed << "ns");
	Message2(Log, Storage, "\tResulting RTree height: " << rt.height());
	
	typename RTree::BoundType bound = rt.bounds();
	
	class Visitor : public RTree::SearchVisitorType {
	public:
		int i;
		virtual ~Visitor() {}
		virtual void visit(const typename RTree::BoundType &, int) {
			i ++;
		}
		
		virtual void visit(const typename RTree::PointType &, int) { }
	};
	
	clock_gettime(CLOCK_REALTIME, &start);
	
	for(int i = 0; i < 50; i ++) {
		Visitor v;
		v.i = 0;
		rt.search(bound, v);
		if(v.i != 1000000) Message2(Warning, Storage, "Complete search did not encounter all items!");
	}
	clock_gettime(CLOCK_REALTIME, &end);
	
	elapsed = (end.tv_sec*1000000000+end.tv_nsec)-(start.tv_sec*1000000000+start.tv_nsec);
	Message2(Log, Storage, "\tElapsed time for 50 complete searches: " << elapsed << "ns");
}

void benchmarkTimings() {
	Message2(Log, Storage, "Running R-tree benchmark . . .");
	
#define benchmark(n) do { \
	benchmarkTimingsFor<n, n*2>(); \
	benchmarkTimingsFor<n, n*3>(); \
	benchmarkTimingsFor<n, n*4>(); \
	benchmarkTimingsFor<n, n*5>(); \
	benchmarkTimingsFor<n, n*6>(); \
	benchmarkTimingsFor<n, n*7>(); \
	benchmarkTimingsFor<n, n*8>(); \
	} while(0)
	
	benchmark(2);
	benchmark(4);
	benchmark(8);
	benchmark(16);
	benchmark(32);
	benchmark(64);
}

} // namespace RTreePrivate
} // namespace Storage
