#ifndef AESALON_ANALYZER_FILE_H
#define AESALON_ANALYZER_FILE_H

#include <string>

namespace Analyzer {

class File {
private:
    std::string filename;
public:
    File(std::string filename);
    ~File();
};

} // namespace Analyzer

#endif
