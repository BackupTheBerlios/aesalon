/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/storage/RTree.h

*/

#ifndef AesalonVisualizer_Storage_RTree_H
#define AesalonVisualizer_Storage_RTree_H

namespace Visualizer {
namespace Storage {

template<typename RangeType, typename StorageType, int Dimensions>
class RTree {
public:
	class Range {
	private:
		RangeType m_from;
		RangeType m_to;
	public:
		Range(RangeType from, RangeType to) : m_from(from), m_to(to) {}
		
		RangeType from() const { return m_from; }
		RangeType to() const { return m_to; }
	};
	
	class Bound {
	private:
		Range m_range[Dimensions];
	public:
		Bound(Range ranges[Dimensions]) {
			for(int i = 0; i < Dimensions; i ++) {
				m_range[i] = ranges[i];
			}
		}
		
		void setRange(int dimension, Range newRange) {
			m_range[dimension] = newRange;
		}
		const Range &range(int dimension) const {
			return m_range[dimension];
		}
	};
protected:
	class Node {
	private:
		Bound m_bound;
	public:
		
	};
private:
	
public:
	RTree();
	~RTree();
};

} // namespace Storage
} // namespace Visualizer

#endif
