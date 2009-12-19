#ifndef AESALON_INTERFACE_ELF_PARSER_H
#define AESALON_INTERFACE_ELF_PARSER_H

#include <string>
#include <vector>
#include <linux/elf.h>

#include "Section.h"
#include "Header.h"

#include "misc/SmartPointer.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Interface {
namespace ELF {

class ParserException : public Misc::Exception {
public:
    ParserException(std::string message) : Misc::Exception(message) {}
    virtual ~ParserException() {}
};

class Parser {
public:
    typedef std::vector<Misc::SmartPointer<Section> > section_list_t;
private:
    std::string filename;
    
    Misc::SmartPointer<Header> header;
    
    section_list_t section_list;
    
    int file_fd;
public:
    Parser(std::string filename);
    virtual ~Parser() {}
};

} // namespace ELF
} // namespace Interface
} // namespace Aesalon

#endif
