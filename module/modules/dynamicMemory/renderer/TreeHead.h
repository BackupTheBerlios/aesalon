#ifndef TreeHead_H
#define TreeHead_H

#include "DataTypes.h"

#include "TreeNode.h"

class TreeHead {
public:
	TreeHead(uint32_t headID);
	~TreeHead();
private:
	uint32_t m_headID;
	TreeNode *m_headNode;
	static uint64_t m_headValue;
	/* TODO: store events that change the previous head into this one. (e.g. delta of headID-1 to headID.) */
	
	enum LookupMode {
		CREATE = 0x01,
		REMOVE = 0x02
	};
public:
	TreeNode *headNode() const { return m_headNode; }
	
	void attachTo(TreeHead *tree);
	
	TreeNode *lookup(uint64_t address);
	TreeNode *create(uint64_t address);
	void remove(uint64_t address);
	void mark(uint64_t address);
private:
	TreeNode *lookup(uint64_t address, int lookupMode);
};

#endif
