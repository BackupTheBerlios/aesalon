#ifndef AESALON_ANALYZER_FILE_H
#define AESALON_ANALYZER_FILE_H

#include <map>
#include <string>

#include "StorageOffset.h"
#include "Parser.h"
#include "InterfaceTypes.h"

namespace Analyzer {

class StorageManager;

class File {
private:
    std::string filename;
    
    StorageOffset sections, symbols, attributes;
    StorageOffset last_section, last_symbol, last_attribute;
    mutable std::map<std::string, StorageOffset> section_cache, symbol_cache, attribute_cache;
    StorageManager *storage_manager;
public:
    File(std::string filename, StorageManager *storage_manager);
    ~File();
    
    const std::string &get_filename() const { return filename; }
    StorageManager *get_storage_manager() const { return storage_manager; }
    
    Object get_section(const char *name) const;
    Object get_symbol(const char *name) const;
    Object get_symbol_for(Word address);
    
    /*StorageOffset *get_section(std::string name) const;*/
    
    /*Symbol *get_symbol(std::string name) const;*/
    StorageOffset get_section_offset(const char *name) const;
    
    StorageOffset get_symbol_offset(const char *name) const;
    Word get_symbol_address(const char *name) const;
    Word get_attribute(const char *name) const;
    
    void set_sections(StorageOffset sections);
    void add_symbol(StorageOffset section);
    void add_attribute(StorageOffset section);
private:
    void parse();
};

} // namespace Analyzer

#endif
