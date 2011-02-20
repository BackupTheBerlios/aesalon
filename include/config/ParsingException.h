/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/config/ParsingException.h
*/

#ifndef AesalonConfig_ParsingException_H
#define AesalonConfig_ParsingException_H

#include "Exception.h"

namespace Config {

class ParsingException : public Exception {
public:
	ParsingException(std::string message) : Exception("Parsing exception: " + message) {}
	virtual ~ParsingException() {}
};

} // namespace Config

#endif

