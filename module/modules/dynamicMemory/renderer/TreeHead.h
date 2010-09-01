#ifndef TreeHead_H
#define TreeHead_H

#include <list>

#include "DataTypes.h"
#include "TreeNode.h"
#include "Event.h"

class TreeHead {
public:
	typedef std::list<Event *> EventList;
	TreeHead(uint32_t headID);
	~TreeHead();
private:
	enum LookupMode {
		CREATE = 0x01,
		REMOVE = 0x02
	};
	
	uint32_t m_headID;
	TreeNode *m_headNode;
	static uint64_t m_headValue;
	EventList m_eventList;
public:
	TreeNode *headNode() const { return m_headNode; }
	
	void attachTo(TreeHead *tree);
	
	void addEvent(Event *event);
	int eventCount() const { return m_eventList.size(); }
	
	TreeNode *lookup(uint64_t address);
	TreeNode *create(uint64_t address);
	void remove(uint64_t address);
	void mark(uint64_t address);
private:
	TreeNode *lookup(uint64_t address, int lookupMode);
};

#endif
