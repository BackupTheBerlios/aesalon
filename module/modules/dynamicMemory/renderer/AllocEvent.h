#ifndef AllocEvent_H
#define AllocEvent_H

#include "DataTypes.h"

#include "Event.h"

class AllocEvent : public Event {
public:
	AllocEvent(uint64_t address, uint64_t size, uint64_t timestamp);
	virtual ~AllocEvent();
private:
	uint64_t m_address;
	uint64_t m_size;
	uint64_t m_timestamp;
public:
	virtual uint64_t timestamp() const { return m_timestamp; }
	
	virtual void accept(EventVisitor *visitor);
	
	virtual void applyTo(TreeHead *head);
};

#endif
