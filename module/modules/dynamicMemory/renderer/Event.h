#ifndef Event_H
#define Event_H

class TreeHead;

class Event {
public:
	virtual ~Event() {}
	
	virtual void applyTo(TreeHead *head) = 0;
};

#endif
