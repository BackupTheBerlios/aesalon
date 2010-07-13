#ifndef Logger_H
#define Logger_H

#include "DataTypes.h"

namespace Program {

class Logger {
public:
	Logger();
	~Logger();
private:
	int m_fd;
public:
	void logPacket(DataPacket *packet);
};

} // namespace Program

#endif
