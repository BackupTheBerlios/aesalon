#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <iostream>

#include "File.h"
#include "Initializer.h"
#include "exception/AnalyzerException.h"

#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86
    #include "ElfParser.h"
#endif

namespace Analyzer {

File::File(std::string filename, StorageManager *storage_manager) : filename(filename), storage_manager(storage_manager) {
    sections = symbols = attributes = -1;
    last_section = last_symbol = last_attribute = -1;
    parse();
}

File::~File() {
}

StorageOffset File::get_section_offset(const char *name) const {
    StorageOffset offset = section_cache[name];
    if(offset == 0) {
        offset = storage_manager->get_from_list(sections, name);
        section_cache[name] = offset;
    }
    return offset;
}

StorageOffset File::get_symbol_offset(const char *name) const {
    StorageOffset offset = symbol_cache[name];
    if(offset == 0) {
        offset = storage_manager->get_from_list(symbols, name);
        symbol_cache[name] = offset;
    }
    return offset;
}

Word File::get_symbol_address(const char *name) const {
    StorageOffset offset = get_symbol_offset(name);
    if(offset == -1) return 0;
    StorageOffset address_offset = storage_manager->get_child(offset, "address");
    if(address_offset == -1) return 0;
    
    return storage_manager->dereference_attribute(address_offset)->value;
}

Word File::get_attribute(const char *name) const {
    StorageOffset offset = attribute_cache[name];
    StorageAttribute *attribute = NULL;
    if(offset == 0) {
        offset = storage_manager->get_from_list(attributes, name);
    }
    if(offset == -1) {
        return 0;
    }
    attribute = storage_manager->dereference_attribute(offset);
    
    return attribute->value;
}

void File::set_sections(StorageOffset sections) {
    this->sections = sections;
}

void File::add_symbol(StorageOffset symbol) {
    if(last_symbol == -1) {
        symbols = symbol;
    }
    else {
        StorageAttribute *last = storage_manager->dereference_attribute(last_symbol);
        last->next = symbol;
    }
    last_symbol = symbol;
}

void File::add_attribute(StorageOffset attribute) {
    if(last_attribute == -1) {
        attributes = attribute;
    }
    else {
        StorageAttribute *last = storage_manager->dereference_attribute(last_attribute);
        last->next = attribute;
    }
    last_attribute = attribute;
}

void File::parse() {
    int file_fd = open(filename.c_str(), O_RDONLY);
    if(file_fd == -1) {
        throw Exception::AnalyzerException(Misc::StreamAsString() << "Couldn't open file for parsing: " << strerror(errno));
    }
    
    int parses = 0;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86
    Parser *elf_parser = new ElfParser(file_fd, this);
    if(elf_parser->parse()) parses ++;
    delete elf_parser;
#endif
    close(file_fd);
    if(parses == 0) {
        throw Exception::AnalyzerException(Misc::StreamAsString() << "Couldn't parse file \"" << filename << "\" . . .");
    }
}

} // namespace Analyzer
