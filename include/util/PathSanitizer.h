/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/util/PathSanitizer.h
*/

#ifndef AesalonUtil_PathSanitizer_H
#define AesalonUtil_PathSanitizer_H

#include <string>
#include <vector>

namespace Util {

class PathSanitizer {
public:
	static std::string sanitize(const std::string &filename);
	static std::string sanitize(const std::string &filename, const std::vector<std::string> &pathList);
private:
	static std::string makeReal(const std::string &path);
};

} // namespace Util

#endif

