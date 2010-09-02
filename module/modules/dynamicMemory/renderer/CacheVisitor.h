#ifndef CacheVisitor_H
#define CacheVisitor_H

#include "Block.h"

class CacheVisitor {
public:
	virtual ~CacheVisitor() {}
	
	virtual void handleBlock(Block *block) = 0;
};

#endif
