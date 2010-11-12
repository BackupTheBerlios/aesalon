/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/PathSanitizer.h

*/

#ifndef AesalonCommon_PathSanitizer_H
#define AesalonCommon_PathSanitizer_H

#include <string>
#include <vector>
#include <sys/stat.h>
#include <cstdlib>

namespace Common {

class PathSanitizer {
public:
	static std::string sanitize(const std::string &filename) {
		if(filename[0] == '/') return filename;
		
		if(filename[0] == '~') {
			char *homeDirectory = std::getenv("HOME");
			if(homeDirectory == NULL) return filename;
			std::string path = filename;
			path.replace(0, 1, homeDirectory);
			return path;
		}
		return filename;
	}
	static std::string sanitize(const std::string &filename, const std::vector<std::string> &pathList) {
		std::string sanitized = sanitize(filename);
		for(std::vector<std::string>::const_iterator i = pathList.begin(); i != pathList.end(); ++i) {
			std::string fullPath = *i + "/";
			fullPath += sanitized;
			struct stat possibleStat;
			if(stat(fullPath.c_str(), &possibleStat) == 0) return fullPath;
		}
		
		return filename;
	}
};

} // namespace Common

#endif
