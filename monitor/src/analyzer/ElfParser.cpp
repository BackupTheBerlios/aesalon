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
    StorageManager *sm = file->get_storage_manager();
    /* Read in the header . . . */
    Elf32_Ehdr header;
    if(read(file_fd, &header, sizeof(header)) != sizeof(header)) {
        status = false;
        return;
    }
    
    /* Start off by parsing the sections . . . */
    lseek(file_fd, header.e_shoff, SEEK_SET);
    
    std::vector<StorageItem *> item_list;
    
    Elf32_Shdr section;
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
        
        item_list.push_back(item);
    }
    
    /* Read the section string table . . . */
    StorageItem *shstrtab = item_list[header.e_shstrndx];
    if(!shstrtab) {
        /* . . . then something is rather wrong. */
        status = false;
        return;
    }
    
    StorageString *shstrtab_content = read_content(shstrtab);
    if(shstrtab_content == NULL) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "Could not read content of string table. Perhaps it is a corrupt ELF file?");
        status = false;
        return;
    }
    for(std::vector<StorageItem *>::iterator i = item_list.begin(); i != item_list.end(); i ++) {
        StorageItem *section = *i;
        StorageAttribute *name = section->get_attribute("name");
        /* name->value stores the string table offset, so add it to the string table content . . . */
        name->set_value(Word(shstrtab_content + name->get_value()));
        file->add_section(section);
    }
    
    item_list.clear();
    
    /* Parse the symbols now. Note that not every ELF file has static and/or dynamic symbols.
        Some have one, some have both. Some have none, too. */
    /* Try the static symbols . . . */
    StorageItem *symtab = file->get_section(".symtab");
    StorageItem *strtab = file->get_section(".strtab");
    if(symtab && strtab) {
        StorageString *symtab_content = read_content(symtab);
        StorageString *strtab_content = read_content(strtab);
        Word symtab_size = symtab->get_attribute("size")->get_value();
        if(symtab_content == NULL || strtab_content == NULL) {
            status = false;
            return;
        }
        Elf32_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(symtab_size)) {
            memcpy(&symbol, symtab_content + offset, sizeof(symbol));
            
            StorageItem *item = sm->new_item();
            
            StorageAttribute *name = sm->new_attribute();
            name->set_name(sm->new_string("name"));
            name->set_value(Word(strtab_content + symbol.st_name));
            /*std::cout << "Found static symbol, name is \"" << (char *)name->get_value() << "\"" << std::endl;*/
            item->add_attribute(name);
            
            StorageAttribute *address = sm->new_attribute();
            address->set_name(sm->new_string("address"));
            address->set_value(symbol.st_value);
            item->add_attribute(address);
            
            StorageAttribute *size = sm->new_attribute();
            size->set_name(sm->new_string("size"));
            size->set_value(section.sh_size);
            item->add_attribute(size);
            
            file->add_symbol(item);
            
            offset += sizeof(symbol);
        }
    }
    
    /* Now try the dynamic symbols . . . */
    StorageItem *dynsym = file->get_section(".dynsym");
    StorageItem *dynstr = file->get_section(".dynstr");
    if(dynsym && dynstr) {
        StorageString *dynsym_content = read_content(dynsym);
        StorageString *dynstr_content = read_content(dynstr);
        Word dynsym_size = dynsym->get_attribute("size")->get_value();
        if(dynsym_content == NULL || dynstr_content == NULL) {
            status = false;
            return;
        }
        Elf32_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(dynsym_size)) {
            memcpy(&symbol, dynsym_content + offset, sizeof(symbol));
            
            StorageItem *item = sm->new_item();
            
            StorageAttribute *name = sm->new_attribute();
            name->set_name(sm->new_string("name"));
            name->set_value(Word(dynstr_content + symbol.st_name));
            /*std::cout << "Found dynamic symbol, name is \"" << (char *)name->get_value() << "\"" << std::endl;*/
            item->add_attribute(name);
            
            StorageAttribute *address = sm->new_attribute();
            address->set_name(sm->new_string("address"));
            address->set_value(symbol.st_value);
            /*std::cout << "\taddress is " << std::hex << symbol.st_value << std::endl;*/
            item->add_attribute(address);
            
            StorageAttribute *size = sm->new_attribute();
            size->set_name(sm->new_string("size"));
            size->set_value(section.sh_size);
            item->add_attribute(size);
            
            file->add_symbol(item);
            
            offset += sizeof(symbol);
        }
    }
}

void ElfParser::parse_64() {
    StorageManager *sm = file->get_storage_manager();
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
    Misc::Message(Misc::Message::WARNING_MESSAGE,
        "Parsing 64-bit elf file, but Aesalon compiled for 32-bit GNU/Linux. "
        "Please recompile Aesalon for a 64-bit platform and try again.");
    status = false;
    return;
#endif
    /* Read in the header . . . */
    Elf64_Ehdr header;
    if(read(file_fd, &header, sizeof(header)) != sizeof(header)) {
        status = false;
        return;
    }
    
    /* Start off by parsing the sections . . . */
    lseek(file_fd, header.e_shoff, SEEK_SET);
    
    std::vector<StorageItem *> item_list;
    
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
        
        item_list.push_back(item);
    }
    
    /* Read the section string table . . . */
    StorageItem *shstrtab = item_list[header.e_shstrndx];
    if(!shstrtab) {
        /* . . . then something is rather wrong. */
        status = false;
        return;
    }
    
    StorageString *shstrtab_content = read_content(shstrtab);
    if(shstrtab_content == NULL) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "Could not read content of string table. Perhaps it is a corrupt ELF file?");
        status = false;
        return;
    }
    for(std::vector<StorageItem *>::iterator i = item_list.begin(); i != item_list.end(); i ++) {
        StorageItem *section = *i;
        StorageAttribute *name = section->get_attribute("name");
        /* name->value stores the string table offset, so add it to the string table content . . . */
        name->set_value(Word(shstrtab_content + name->get_value()));
        file->add_section(section);
    }
    
    item_list.clear();
    
    /* Parse the symbols now. Note that not every ELF file has static and/or dynamic symbols.
        Some have one, some have both. Some have none, too. */
    /* Try the static symbols . . . */
    StorageItem *symtab = file->get_section(".symtab");
    StorageItem *strtab = file->get_section(".strtab");
    if(symtab && strtab) {
        StorageString *symtab_content = read_content(symtab);
        StorageString *strtab_content = read_content(strtab);
        Word symtab_size = symtab->get_attribute("size")->get_value();
        if(symtab_content == NULL || strtab_content == NULL) {
            status = false;
            return;
        }
        Elf64_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(symtab_size)) {
            memcpy(&symbol, symtab_content + offset, sizeof(symbol));
            
            StorageItem *item = sm->new_item();
            
            StorageAttribute *name = sm->new_attribute();
            name->set_name(sm->new_string("name"));
            name->set_value(Word(strtab_content + symbol.st_name));
            /*std::cout << "Found static symbol, name is \"" << (char *)name->get_value() << "\"" << std::endl;*/
            item->add_attribute(name);
            
            StorageAttribute *address = sm->new_attribute();
            address->set_name(sm->new_string("address"));
            address->set_value(symbol.st_value);
            item->add_attribute(address);
            
            StorageAttribute *size = sm->new_attribute();
            size->set_name(sm->new_string("size"));
            size->set_value(section.sh_size);
            item->add_attribute(size);
            
            file->add_symbol(item);
            
            offset += sizeof(symbol);
        }
    }
    
    /* Now try the dynamic symbols . . . */
    StorageItem *dynsym = file->get_section(".dynsym");
    StorageItem *dynstr = file->get_section(".dynstr");
    if(dynsym && dynstr) {
        StorageString *dynsym_content = read_content(dynsym);
        StorageString *dynstr_content = read_content(dynstr);
        Word dynsym_size = dynsym->get_attribute("size")->get_value();
        if(dynsym_content == NULL || dynstr_content == NULL) {
            status = false;
            return;
        }
        Elf64_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(dynsym_size)) {
            memcpy(&symbol, dynsym_content + offset, sizeof(symbol));
            
            StorageItem *item = sm->new_item();
            
            StorageAttribute *name = sm->new_attribute();
            name->set_name(sm->new_string("name"));
            name->set_value(Word(dynstr_content + symbol.st_name));
            /*std::cout << "Found dynamic symbol, name is \"" << (char *)name->get_value() << "\"" << std::endl;*/
            item->add_attribute(name);
            
            StorageAttribute *address = sm->new_attribute();
            address->set_name(sm->new_string("address"));
            address->set_value(symbol.st_value);
            /*std::cout << "\taddress is " << std::hex << symbol.st_value << std::endl;*/
            item->add_attribute(address);
            
            StorageAttribute *size = sm->new_attribute();
            size->set_name(sm->new_string("size"));
            size->set_value(section.sh_size);
            item->add_attribute(size);
            
            file->add_symbol(item);
            
            offset += sizeof(symbol);
        }
    }
}

StorageString *ElfParser::read_content(StorageItem *section) {
    StorageManager *sm = file->get_storage_manager();
    
    if(section->get_attribute("data") != NULL) {
        return (StorageString *)section->get_attribute("data")->get_value();
    }
    
    Word data_size = section->get_attribute("size")->get_value();
    StorageString *data = sm->new_string(data_size);
    
    lseek(file_fd, section->get_attribute("offset")->get_value(), SEEK_SET);
    if(read(file_fd, data, data_size) != (SWord)data_size) return 0;
    
    StorageAttribute *data_attribute = sm->new_attribute();
    data_attribute->set_name(sm->new_string("data"));
    data_attribute->set_value(reinterpret_cast<Word>(data));
    section->add_attribute(data_attribute);
    
    return data;
}

} // namespace Analyzer

/*#endif*/
