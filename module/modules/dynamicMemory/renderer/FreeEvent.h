#ifndef FreeEvent_H
#define FreeEvent_H

#include "DataTypes.h"

#include "Event.h"

class FreeEvent : public Event {
public:
	FreeEvent(uint64_t address, uint64_t timestamp);
	virtual ~FreeEvent();
private:
	uint64_t m_address;
	uint64_t m_timestamp;
public:
    virtual void applyTo(TreeHead *head);
};

#endif
