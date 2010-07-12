#include <stdlib.h>
#include <sys/stat.h>

#include <iostream>

#include "PathSanitizer.h"

namespace Misc {

std::string PathSanitizer::sanitize(std::string path) {
	char *homeDirectory = getenv("HOME");
	
	if(path[0] == '~') path.replace(0, 1, homeDirectory);
	
	return path;
}

std::string PathSanitizer::findFromPaths(std::string filename, std::string searchPaths) {
	if(searchPaths.length() == 0 || filename.length() == 0) return "";
	
	do {
		std::string path = searchPaths.substr(0, searchPaths.find(":"));
		
		path += '/';
		path += filename;
		struct stat possibleStat;
		if(stat(path.c_str(), &possibleStat) == 0) return path;
		
		searchPaths.erase(0, path.length() - filename.length());
	} while(searchPaths.find(":") != std::string::npos);
	
	return "";
}

} // namespace Misc
