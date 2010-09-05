#ifndef Block_H
#define Block_H

#include "DataTypes.h"

class Block {
public:
	Block(uint64_t address, uint64_t size, Timestamp allocTime);
private:
	uint64_t m_address;
	uint64_t m_size;
	Timestamp m_allocTime;
	Timestamp m_releaseTime;
	Block *m_reallocedTo;
public:
	uint64_t address() const { return m_address; }
	uint64_t size() const { return m_size; }
	Timestamp allocTime() const { return m_allocTime; }
	Timestamp releaseTime() const { return m_releaseTime; }
	void setReleaseTime(uint64_t releaseTime) { m_releaseTime = releaseTime; }
	
	Block *reallocedTo() const { return m_reallocedTo; }
	void setReallocedTo(Block *reallocedTo) { m_reallocedTo = reallocedTo; }
};

#endif
