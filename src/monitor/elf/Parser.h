#ifndef AESALON_MONITOR_ELF_PARSER_H
#define AESALON_MONITOR_ELF_PARSER_H

#include <string>
#include <vector>
#include <linux/elf.h>

#include "Section.h"
#include "Header.h"

#include "misc/SmartPointer.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Monitor {
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
    Misc::SmartPointer<Section> string_table;
    
    int file_fd;
public:
    Parser(std::string filename);
    virtual ~Parser() {}
    
    Misc::SmartPointer<Section> get_section(std::string name) const;
};

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon

#endif