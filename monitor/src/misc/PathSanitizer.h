#ifndef PathSanitizer_H
#define PathSanitizer_H

#include <string>

namespace Misc {

class PathSanitizer {
public:
	/** Sanitizes a path. Replaces ~ with /home/username/, etc.
		@param path The path to sanitize.
		@return @a path, sanitized.
	*/
	static std::string sanitize(std::string path);
	/** Finds a file in a list of paths.
		@param filename The name of the file to find.
		@param searchPaths A colon-separated list of paths to search.
		@return The path to the filename, or "" if the file was not found.
	*/
	static std::string findFromPaths(std::string filename, std::string searchPaths);
};

} // namespace Misc

#endif
