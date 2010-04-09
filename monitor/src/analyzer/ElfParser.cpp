/* TODO: uncomment these . . . */
/*#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86*/

#include <sys/types.h>
#include <linux/elf.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <iostream>

#include "ElfParser.h"
#include "misc/Message.h"
#include "misc/StreamAsString.h"
#include "Types.h"
#include "File.h"
#include "StorageManager.h"

namespace Analyzer {

ElfParser::ElfParser(int file_fd, File *file) : Parser(file_fd, file) {

}

ElfParser::~ElfParser() {

}

bool ElfParser::parse() {
    std::cout << "Parsing ELF file . . ." << std::endl;
    Byte ident[EI_NIDENT];
    
    lseek(file_fd, 0, SEEK_SET);
    if(read(file_fd, ident, sizeof(ident)) != sizeof(ident)) {
        Misc::Message(Misc::Message::DEBUG_MESSAGE, "Couldn't read ELF identification . . .");
        return false;
    }
    if(memcmp(ident, ELFMAG, SELFMAG) != 0) {
        Misc::Message(Misc::Message::DEBUG_MESSAGE, "Elf magic incorrect . . .");
        return false;
    }
    
    lseek(file_fd, 0, SEEK_SET);
    
    status = true;
    
    Byte elf_class = ident[EI_CLASS];
    if(elf_class == ELFCLASS32) parse_32();
    else if(elf_class == ELFCLASS64) parse_64();
    else {
        Misc::Message(Misc::Message::DEBUG_MESSAGE, "Unknown ELF class encountered . . .");
        return false;
    }
    
    close(file_fd);
    return status;
}

void ElfParser::parse_32() {
    Misc::Message(Misc::Message::WARNING_MESSAGE, "32-bit ELF parsing NYI!");
    status = false;
}

void ElfParser::parse_64() {
    StorageManager *sm = file->get_storage_manager();
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
    Misc::Message(Misc::Message::WARNING_MESSAGE,
        "Parsing 64-bit elf file, but Aesalon compiled for 32-bit GNU/Linux. "
        "Be warned: Aesalon will probably not work.");
#endif
    Elf64_Ehdr header;
    if(read(file_fd, &header, sizeof(header)) != sizeof(header)) {
        status = false;
        return;
    }
    
    lseek(file_fd, header.e_shoff, SEEK_SET);
    
    std::vector<StorageItem *> sections;
    
    Elf64_Shdr section;
    for(int s = 0; s < header.e_shnum; s ++) {
        if(read(file_fd, &section, sizeof(section)) != sizeof(section)) {
            status = false;
            return;
        }
        StorageItem *item = sm->new_item();
        StorageAttribute *offset = sm->new_attribute();
        offset->set_name(sm->new_string("offset"));
        offset->set_value(section.sh_offset);
        item->add_attribute(offset);
        
        StorageAttribute *address = sm->new_attribute();
        address->set_name(sm->new_string("address"));
        address->set_value(section.sh_addr);
        item->add_attribute(address);
        
        StorageAttribute *size = sm->new_attribute();
        size->set_name(sm->new_string("size"));
        size->set_value(section.sh_size);
        item->add_attribute(size);
        
        StorageAttribute *name = sm->new_attribute();
        name->set_name(sm->new_string("name"));
        name->set_value(section.sh_name);
        item->add_attribute(name);
        
        sections.push_back(item);
    }
    
    StorageItem *strtab = sections[header.e_shstrndx];
    if(strtab) {
        StorageString *strtab_content = read_content(strtab);
        if(!strtab_content) {
            status = false;
            return;
        }
        for(std::vector<StorageItem *>::iterator i = sections.begin(); i != sections.end(); i ++) {
            StorageItem *item = *i;
            StorageAttribute *name = item->get_attribute("name");
            /* name->value stores the string table offset, so add it to the string table content . . . */
            name->set_value(Word(strtab_content + name->get_value()));
        }
    }
}

StorageString *ElfParser::read_content(StorageItem *section) {
    StorageManager *sm = file->get_storage_manager();
    
    Word data_size = section->get_attribute("size")->get_value();
    StorageString *data = sm->new_string(data_size);
    
    lseek(file_fd, section->get_attribute("offset")->get_value(), SEEK_SET);
    if(read(file_fd, data, data_size) != SWord(data_size)) return 0;
    
    StorageAttribute *data_attribute = sm->new_attribute();
    data_attribute->set_name(sm->new_string("data"));
    data_attribute->set_value(reinterpret_cast<Word>(data));
    section->add_attribute(data_attribute);
    
    return data;
}

} // namespace Analyzer

/*#endif*/
