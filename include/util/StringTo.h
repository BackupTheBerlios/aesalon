/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/util/StringTo.h
*/

#ifndef AesalonUtil_StringTo_H
#define AesalonUtil_StringTo_H

#include <sstream>

namespace Util {

template<typename Type>
Type StringTo(const std::string &string) {
	if(string == "") return Type();
	std::istringstream ss(string);
	Type t;
	ss >> t;
	return t;
}

} // namespace Util

#endif
