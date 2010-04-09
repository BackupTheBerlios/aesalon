#ifndef AESALON_ANALYZER_PARSER_H
#define AESALON_ANALYZER_PARSER_H

#include <string>

namespace Analyzer {

class File;

class Parser {
protected:
    int file_fd;
    File *file;
public:
    Parser(int file_fd, File *file);
    virtual ~Parser();
    
    virtual bool parse() = 0;
};

} // namespace Analyzer

#endif
