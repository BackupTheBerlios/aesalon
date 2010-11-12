/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/NYIException.h

*/

#ifndef AesalonCommon_NYIException_H
#define AesalonCommon_NYIException_H

#include "Exception.h"

namespace Common {

class NYIException : public Exception {
public:
	NYIException(std::string message) : Exception("NYI: " + message) {}
	virtual ~NYIException() {}
};

} // namespace Common

#endif
