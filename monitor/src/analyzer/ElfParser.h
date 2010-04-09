#ifndef AESALON_ANALYZER_ELF_PARSER_H
#define AESALON_ANALYZER_ELF_PARSER_H

/*#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86*/

#include "StorageOffset.h"
#include "Parser.h"

namespace Analyzer {

class ElfParser : public Parser {
private:
    bool status;
public:
    ElfParser(int file_fd, File *file);
    virtual ~ElfParser();
    
    virtual bool parse();
private:
    void parse_32();
    void parse_64();
    
    StorageOffset read_content(StorageOffset section);
};

} // namespace Analyzer

/*#endif*/

#endif
