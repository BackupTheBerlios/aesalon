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
    parse();
}

File::~File() {
    for(item_map_t::const_iterator i = sections.begin(); i != sections.end(); i ++) {
        i->second->~StorageObject();
    }
    for(item_map_t::const_iterator i = symbols.begin(); i != symbols.end(); i ++) {
        i->second->~StorageObject();
    }
    for(item_map_t::const_iterator i = attributes.begin(); i != attributes.end(); i ++) {
        i->second->~StorageObject();
    }
}

StorageItem *File::get_section(std::string name) const {
    StorageItem *item = section_cache[name];
    if(item != NULL) return item;
    for(item_map_t::const_iterator i = sections.begin(); i != sections.end(); i ++) {
        if(!strcmp(i->first, name.c_str())) {
            item = i->second;
            break;
        }
    }
    if(item == NULL) return NULL;
    section_cache[name] = item;
    return item;
}

StorageItem *File::get_symbol(std::string name) const {
    StorageItem *item = symbol_cache[name];
    if(item != NULL) return item;
    for(item_map_t::const_iterator i = symbols.begin(); i != symbols.end(); i ++) {
        if(!strcmp(i->first, name.c_str())) {
            item = i->second;
            break;
        }
    }
    if(item == NULL) return NULL;
    symbol_cache[name] = item;
    return item;
}

Word File::get_symbol_address(std::string name) const {
    StorageItem *item = get_symbol(name);
    if(item == NULL) return 0;
    return item->get_attribute("address")->get_value();
}

StorageItem *File::get_attribute(std::string name) const {
    StorageItem *item = attribute_cache[name];
    if(item != NULL) return item;
    for(item_map_t::const_iterator i = attributes.begin(); i != attributes.end(); i ++) {
        if(!strcmp(i->first, name.c_str())) {
            item = i->second;
            break;
        }
    }
    if(item == NULL) return NULL;
    attribute_cache[name] = item;
    return item;
}

void File::add_section(StorageItem *item) {
    sections[(StorageString *)item->get_attribute("name")->get_value()] = item;
}

void File::add_symbol(StorageItem *item) {
    symbols[(StorageString *)item->get_attribute("name")->get_value()] = item;
}

void File::add_attribute(StorageItem *item) {
    attributes[(StorageString *)item->get_attribute("name")->get_value()] = item;
}

void File::parse() {
    int file_fd = open(filename.c_str(), O_RDONLY);
    if(file_fd == -1) {
        throw Exception::AnalyzerException(Misc::StreamAsString() << "Couldn't open file for parsing: " << strerror(errno));
    }
    
    int parses = 0;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
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
