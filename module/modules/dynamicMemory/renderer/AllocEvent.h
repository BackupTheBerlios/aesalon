#ifndef AllocEvent_H
#define AllocEvent_H

#include "Event.h"

class AllocEvent : public Event {
public:
	AllocEvent();
	virtual ~AllocEvent();
private:
public:
	virtual void applyTo(TreeHead *head);
};

#endif
