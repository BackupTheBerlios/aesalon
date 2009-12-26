#ifndef AESALON_MONITOR_ELF_SECTION_H
#define AESALON_MONITOR_ELF_SECTION_H

#include <linux/elf.h>

#include "Types.h"

#include "misc/SmartPointer.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Monitor {
namespace ELF {

class Section {
private:
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    typedef Elf64_Shdr section_t;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    typedef Elf32_Shdr section_t;
#endif
    
    int file_fd;

    section_t data;
    
    Misc::SmartPointer<Block> content;
    
    std::string name;
    
    std::size_t get_content_size() const { return data.sh_size; }
public:
    Section(int file_fd);
    virtual ~Section() {}
    
    void read_content();
    Misc::SmartPointer<Block> get_content() {
        if(!content.is_valid()) read_content();
        return content;
    }
    
    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }
    
    Platform::MemoryAddress get_name_offset() const { return data.sh_name; }
    
    bool is_string_table() const { return data.sh_type == SHT_STRTAB; }
};

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon

#endif
