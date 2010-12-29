/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/program/SMS.h

*/

#ifndef AesalonMonitor_Program_SMS_H
#define AesalonMonitor_Program_SMS_H

#include <stdint.h>

namespace Monitor {
namespace Program {

class SMS {
private:
	uint64_t m_id;
	uint32_t m_size;
	
	int m_fd;
	uint8_t *m_data;
public:
	SMS(uint64_t id, uint32_t size);
	~SMS();
};

} // namespace Program
} // namespace Monitor

#endif
