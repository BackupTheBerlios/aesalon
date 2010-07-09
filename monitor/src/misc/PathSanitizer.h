#ifndef PathSanitizer_H
#define PathSanitizer_H

#include <string>

namespace Misc {

class PathSanitizer {
public:
	static std::string sanitize(std::string path);
};

} // namespace Misc

#endif
