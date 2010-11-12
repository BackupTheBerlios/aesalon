/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/StringTo.h

*/

#ifndef AesalonCommon_StringTo_H
#define AesalonCommon_StringTo_H

#include <string>
#include <sstream>
#include <iostream>

namespace Common {

/** Converts a string into a given type. Note that @a Type must have a default constructor.
*/
template<typename Type>
Type StringTo(const std::string &string) {
	if(string == "") return Type();
	std::istringstream ss(string);
	Type t;
	ss >> t;
	return t;
}

} // namespace Common

#endif
