#include <iostream>

#include "Interface.h"

namespace Analyzer {

Interface::Interface() {
    storage_manager = new StorageManager();
    first_file = NULL;
}

Interface::~Interface() {
    for(file_map_t::iterator i = file_map.begin(); i != file_map.end(); i ++) {
        delete i->second;
    }

    delete storage_manager;
}

File *Interface::parse_file(std::string filename) {
    if(file_map[filename] != NULL) return file_map[filename];
    
    File *file = new File(filename, storage_manager);
    /* the get_filename() is there because the File parser can change
        the filename to account for relative paths, symlinks, etc. */
    file_map[file->get_filename()] = file;
    if(first_file == NULL) first_file = file;
    return file;
}

File *Interface::get_file(std::string filename) {
    return file_map[filename];
}

File *Interface::get_file() const {
    return first_file;
}

} // namespace Analyzer
