#include <stdlib.h>

#include "PathSanitizer.h"

namespace Misc {

std::string PathSanitizer::sanitize(std::string path) {
	char *homeDirectory = getenv("HOME");
	
	if(path[0] == '~') path.replace(0, 1, homeDirectory);
	
	return path;
}

} // namespace Misc
