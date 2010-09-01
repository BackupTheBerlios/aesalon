#ifndef Event_H
#define Event_H

class TreeHead;
class EventVisitor;

class Event {
public:
	virtual ~Event() {}

	virtual uint64_t timestamp() const = 0;

	virtual void accept(EventVisitor *visitor) = 0;

	virtual void applyTo(TreeHead *head) = 0;
};

#endif
