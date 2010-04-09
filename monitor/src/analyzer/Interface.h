#ifndef AESALON_ANALYZER_INTERFACE_H
#define AESALON_ANALYZER_INTERFACE_H

#include <string>
#include <map>

#include "File.h"
#include "StorageManager.h"

namespace Analyzer {

class Interface {
protected:
    typedef std::map<std::string, File *> file_map_t;
private:
    file_map_t file_map;
    File *first_file;
    
    StorageManager *storage_manager;
public:
    Interface();
    ~Interface();
    
    File *parse_file(std::string filename);
    File *get_file(std::string filename) const;
    File *get_file() const;
    
    StorageManager *get_storage_manager() const { return storage_manager; }
};

} // namespace Analyzer

#endif
