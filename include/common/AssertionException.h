/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/AssertionException.h

*/

#ifndef AesalonCommon_AssertionException_H
#define AesalonCommon_AssertionException_H

#include "Exception.h"

namespace Common {

class AssertionException : public Exception {
public:
	AssertionException(std::string message) : Exception("Assertion failed: " + message) {}
	virtual ~AssertionException() {}
};

} // namespace Common

#endif
