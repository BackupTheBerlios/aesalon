#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cstddef>

#include "File.h"
#include "Initializer.h"
#include "exception/AnalyzerException.h"

#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86
    #include "ElfParser.h"
#endif

namespace Analyzer {

File::File(std::string filename, StorageManager *storage_manager) : filename(filename), storage_manager(storage_manager) {
    sections = symbols = attributes = NULL;
    last_section = last_symbol = last_attribute = NULL;
    parse();
}

File::~File() {

}

Object File::get_section(const char *name) const {
    StorageAttribute *attribute = get_section_attribute(name);
    if(attribute == NULL) return Object("", 0, 0);
    return Object(attribute->get_name(),
        storage_manager->get_child(attribute, "address")->get_value(),
        storage_manager->get_child(attribute, "size")->get_value());
}

Object File::get_symbol(const char *name) const {
    StorageAttribute *attribute = get_symbol_attribute(name);
    if(attribute == NULL) return Object("", 0, 0);
    return Object(attribute->get_name(),
        storage_manager->get_child(attribute, "address")->get_value(),
        storage_manager->get_child(attribute, "size")->get_value());
}

Object File::get_symbol_for(Word address) {
    StorageAttribute *attribute = symbols;
    while(attribute != NULL) {
        Word addr = storage_manager->get_child(attribute, "address")->get_value();
        Word size = storage_manager->get_child(attribute, "size")->get_value();
        if(address >= addr && address <= (addr + size)) {
            return Object(attribute->get_name(), addr, size);
        }
        attribute = attribute->get_next();
    }
    return Object("", 0, 0);
}

StorageAttribute *File::get_section_attribute(const char *name) const {
    StorageAttribute *attribute = section_cache[name];
    if(attribute == NULL) {
        attribute = storage_manager->get_from_list(sections, name);
        section_cache[name] = attribute;
    }
    return attribute;
}

StorageAttribute *File::get_symbol_attribute(const char *name) const {
    StorageAttribute *attribute = symbol_cache[name];
    if(attribute == NULL) {
        attribute = storage_manager->get_from_list(symbols, name);
        symbol_cache[name] = attribute;
    }
    return attribute;
}

Word File::get_symbol_address(const char *name) const {
    StorageAttribute *attribute = get_symbol_attribute(name);
    if(attribute == NULL) return 0;
    
    return storage_manager->get_child(attribute, "address")->get_value();
}

Word File::get_attribute(const char *name) const {
    StorageAttribute *attribute = attribute_cache[name];
    if(attribute == NULL) {
        attribute = storage_manager->get_from_list(attributes, name);
        attribute_cache[name] = attribute;
    }
    if(attribute == NULL) {
        return 0;
    }
    
    return attribute->get_value();
}

void File::set_sections(StorageAttribute *sections) {
    this->sections = sections;
}

void File::add_symbol(StorageAttribute *symbol) {
    if(symbols == NULL) {
        symbols = symbol;
    }
    else {
        last_symbol->set_next(symbol);
    }
    last_symbol = symbol;
}

void File::add_attribute(StorageAttribute *attribute) {
    if(last_attribute == NULL) {
        attributes = attribute;
    }
    else {
        last_attribute->set_next(attribute);
    }
    last_attribute = attribute;
}

void File::parse() {
    /* check for symlinks . . . */
    while(true) {
        char buffer[1024];
        ssize_t ret = readlink(filename.c_str(), buffer, sizeof(buffer));
        if(ret == -1) {
            /* errno will be EINVAL when filename is not a symlink, so break. */
            if(errno == EINVAL) break;
            throw Exception::AnalyzerException(Misc::StreamAsString() << "Couldn't resolve symlink \"" << filename << "\": " << strerror(errno));
        }
        else {
            buffer[ret] = NULL;
            filename = buffer;
        }
    }
    
    /* Fix the path if it's not an absolute path . . . */
    if(filename[0] != '/') {
#ifdef PATH_MAX
        char pwd[PATH_MAX];
#else
        char pwd[1024];
#endif
        filename.insert(0, "/");
        filename.insert(0, getcwd(pwd, sizeof(pwd)));
    }
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
