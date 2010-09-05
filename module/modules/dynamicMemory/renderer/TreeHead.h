#ifndef TreeHead_H
#define TreeHead_H

#include <list>

#include "DataTypes.h"
#include "TreeNode.h"

class TreeHead {
public:
	typedef std::list<Block *> BlockList;
	TreeHead(uint32_t headID);
	~TreeHead();
private:
	enum LookupMode {
		CREATE = 0x01,
		REMOVE = 0x02
	};
	
	uint32_t m_headID;
	TreeNode *m_headNode;
	BlockList m_changedList;
	Timestamp m_timestamp;
public:
	TreeNode *headNode() const { return m_headNode; }
	Timestamp timestamp() const { return m_timestamp; }
	void updateTimestamp(Timestamp timestamp) { m_timestamp = timestamp; }
	
	void attachTo(TreeHead *tree);
	
	const BlockList &changedList() const { return m_changedList; }
	void appendChanged(Block *block) { m_changedList.push_back(block); }
	
	/** Looks up a tree node.
		@param address The address of the node to look up.
		@return The node at @a address, or NULL if there is no block at that location.
	*/
	TreeNode *lookup(uint64_t address);
	
	/** Creates a tree node.
		@param address The address of the new node.
		@return A new node at @a address, or NULL if the node alreay exists.
	*/
	TreeNode *create(uint64_t address);
	
	/** Removes a tree node.
		@param address The address of the node to remove.
		@return True if the node was removed, false if removing the node would destroy data.
	*/
	bool remove(uint64_t address);
	
	/** Marks an address as changed.
		@param address The address to mark.
	*/
	void mark(uint64_t address);
private:
	TreeNode *lookup(uint64_t address, int lookupMode);
};

#endif
