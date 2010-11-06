/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/Exception.h

*/

#ifndef AesalonCommon_Exception_H
#define AesalonCommon_Exception_H

#include <string>

namespace Common {

class Exception {
private:
	std::string m_message;
public:
	Exception(std::string message) : m_message(message) {}
	virtual ~Exception() {}
	
	const std::string &message() const { return m_message; }
};

} // namespace Common

#endif
