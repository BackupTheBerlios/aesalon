/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/util/StringToBool.h
*/

#ifndef AesalonUtil_StringToBool_H
#define AesalonUtil_StringToBool_H

#include <string.h>

/** C version of the StringToBool function. Returns the boolean representation of @a string.
*/
inline int StringToBool(const char *string) {
	if(string != NULL && (strcmp(string, "true") == 0 || strcmp(string, "True") == 0)) return 1;
	return 0;
}

#ifdef __cplusplus

#include <string>

namespace Util {

/** C++ version of the StringToBool function. Returns the boolean representation of @a string. */
bool StringToBool(const std::string &string) {
	if(string.length() == 0) return false;
	return ::StringToBool(string.c_str()) == 1;
}

} // namespace Util

#endif

#endif
