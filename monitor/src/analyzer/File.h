#ifndef AESALON_ANALYZER_FILE_H
#define AESALON_ANALYZER_FILE_H

#include <map>
#include <string>

#include "StorageItem.h"
#include "Parser.h"

namespace Analyzer {

class StorageManager;

class File {
private:
    std::string filename;
    
    typedef std::map<StorageString *, StorageItem *> item_map_t;
    item_map_t sections;
    item_map_t symbols;
    item_map_t attributes;
    mutable std::map<std::string, StorageItem *> section_cache, symbol_cache, attribute_cache;
    StorageManager *storage_manager;
public:
    File(std::string filename, StorageManager *storage_manager);
    ~File();
    
    StorageManager *get_storage_manager() const { return storage_manager; }
    
    StorageItem *get_section(std::string name) const;
    StorageItem *get_symbol(std::string name) const;
    Word get_symbol_address(std::string name) const;
    StorageItem *get_attribute(std::string name) const;
    
    void add_section(StorageItem *item);
    void add_symbol(StorageItem *item);
    void add_attribute(StorageItem *item);
private:
    void parse();
};

} // namespace Analyzer

#endif
