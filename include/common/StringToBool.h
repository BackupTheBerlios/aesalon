/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/StringToBool.h

*/

#ifndef AesalonCommon_StringToBool_H
#define AesalonCommon_StringToBool_H

#include <string.h>

/** C version of the StringToBool function. Returns the boolean representation of @a string.
*/
inline int StringToBool(const char *string) {
	if(strcmp(string, "true") == 0 || strcmp(string, "True") == 0) return 1;
	return 0;
}

#ifdef __cplusplus

#include <string>

namespace Common {

/** C++ version of the StringToBool function. Returns the boolean representation of @a string.
*/
inline bool StringToBool(const std::string &string) {
	if(string.length() == 0) return 0;
	else return StringToBool(string.c_str()) == 1;
}

} // namespace Common

#endif

#endif
