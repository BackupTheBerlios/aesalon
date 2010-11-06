/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/ParsingException.h

*/

#ifndef AesalonCommon_ParsingException_H
#define AesalonCommon_ParsingException_H

#include "Exception.h"

namespace Common {

class ParsingException : public Exception {
public:
	ParsingException(std::string message) : Exception("Parsing exception: " + message) {}
	virtual ~ParsingException() {}
};

} // namespace Common

#endif
