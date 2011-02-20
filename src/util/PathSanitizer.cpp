/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/util/PathSanitizer.cpp
*/

#include <sys/stat.h>
#include <cstdlib>

#include "util/PathSanitizer.h"

namespace Util {

std::string PathSanitizer::sanitize(const std::string &filename) {
	if(filename[0] == '/') return makeReal(filename);
	
	if(filename[0] == '~') {
		char *homeDirectory = std::getenv("HOME");
		if(homeDirectory == NULL) return makeReal(filename);
		std::string path = filename;
		path.replace(0, 1, homeDirectory);
		return makeReal(path);
	}
	
	return makeReal(filename);
}
std::string PathSanitizer::sanitize(const std::string &filename, const std::vector<std::string> &pathList) {
	std::string sanitized = sanitize(filename);
	for(std::vector<std::string>::const_iterator i = pathList.begin(); i != pathList.end(); ++i) {
		std::string fullPath = *i + "/";
		fullPath += sanitized;
		struct stat possibleStat;
		if(stat(fullPath.c_str(), &possibleStat) == 0) return fullPath;
	}
	
	return filename;
}
std::string PathSanitizer::makeReal(const std::string &path) {
	char *pathStr = realpath(path.c_str(), NULL);
	if(pathStr != NULL) {
		std::string temp = pathStr;
		free(pathStr);
		return temp;
	}
	return path;
}


} // namespace Util
