#ifndef AESALON_ANALYZER_FILE_H
#define AESALON_ANALYZER_FILE_H

#include <map>
#include <string>

#include "StorageOffset.h"
#include "StorageAttribute.h"
#include "Parser.h"
#include "InterfaceTypes.h"

namespace Analyzer {

class StorageManager;

class File {
private:
    std::string filename;
    
    StorageAttribute *sections, *symbols, *attributes;
    StorageAttribute *last_section, *last_symbol, *last_attribute;
    mutable std::map<std::string, StorageAttribute *> section_cache, symbol_cache, attribute_cache;
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
    StorageAttribute *get_section_attribute(const char *name) const;
    
    StorageAttribute *get_symbol_attribute(const char *name) const;
    Word get_symbol_address(const char *name) const;
    Word get_attribute(const char *name) const;
    
    void set_sections(StorageAttribute *sections);
    void add_symbol(StorageAttribute *symbol);
    void add_attribute(StorageAttribute *attribute);
private:
    void parse();
};

} // namespace Analyzer

#endif
