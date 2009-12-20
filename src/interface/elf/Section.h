#ifndef AESALON_INTERFACE_ELF_SECTION_H
#define AESALON_INTERFACE_ELF_SECTION_H

#include <linux/elf.h>

#include "misc/SmartPointer.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {
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
    
    Misc::SmartPointer<char> content;
    
    std::string name;
public:
    Section(int file_fd);
    virtual ~Section() {}
    
    void read_content();
    Misc::SmartPointer<char> get_content() const { return content; }
    std::size_t get_content_size() const { return data.sh_size; }
    
    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }
    
    Platform::MemoryAddress get_name_offset() const { return data.sh_name; }
    
    bool is_string_table() const { return data.sh_type == SHT_STRTAB; }
};

} // namespace ELF
} // namespace Interface
} // namespace Aesalon

#endif
