#ifndef EventVisitor_H
#define EventVisitor_H

class AllocEvent;
class FreeEvent;

class EventVisitor {
public:
	virtual ~EventVisitor() {}
	
	virtual void visit(AllocEvent *event) = 0;
	virtual void visit(FreeEvent *event) = 0;
};

#endif
