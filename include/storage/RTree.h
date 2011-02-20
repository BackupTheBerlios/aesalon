/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/RTree.h
*/

#ifndef AesalonStorage_RTree_H
#define AesalonStorage_RTree_H

#include <vector>

namespace Storage {

/** An n-dimensional R-tree.
	Note that Value must be either a basic type or a pointer.
*/
template<typename Key, typename Value, int Dimensions, int Maximum = 8, int Minimum = Maximum/2>
class RTree {
public:
	class Range {
	private:
		Key m_start, m_end;
	public:
		Range(Key start, Key end) : m_start(start), m_end(end) {}
		
		Key start() const { return m_start; }
		Key end() const { return m_end; }
		
		bool overlaps(const Range &other) const {
			if(m_start > other.m_end || other.m_start > m_end) return false;
			return true;
		}
	};
	
	class Bound {
	private:
		Range m_ranges[Dimensions];
	public:
		Bound() {}
		Bound(Range ranges[Dimensions]) : m_ranges(ranges) {}
		
		void setRange(Range &range, int dimension) { m_ranges[dimension] = range; }
		Range &range(int dimension) { return m_ranges[dimension]; }
		
		bool overlaps(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				if(!m_ranges[i].overlaps(other.m_ranges[i])) return false;
			}
			return true;
		}
	};
	
	class SearchProcessor {
	public:
		virtual bool process(const Bound &bound, Value value) = 0;
	};
protected:
	class Node;
	struct Branch {
		Bound bound;
		union {
			Node *node;
			Value value;
		};
	};
	class Node {
	private:
		bool m_isLeaf;
		Branch m_branches[Maximum];
		int m_branchCount;
	public:
		Node(bool isLeaf) : m_isLeaf(isLeaf) {}
		bool isLeaf() const { return m_isLeaf; }
		
		Branch &branch(int which) { return m_branches[which]; }
		int branchCount() const { return m_branchCount; }
	};
private:
	Node *m_root;
public:
	RTree();
	~RTree();
	
	void search(const Bound &bound, SearchProcessor *processor);
private:
	void search(const Bound &bound, Node *node, SearchProcessor *processor);
};

} // namespace Storage

#endif
