/* TODO: uncomment these . . . */
/*#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64 || AESALON_PLATFORM == AESALON_PLATFORM_x86*/

#include <sys/types.h>
#include <linux/elf.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <cxxabi.h>
#include <stdlib.h>

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
    
    /* Set platform "bits" attribute */
    StorageManager *sm = file->get_storage_manager();
    StorageAttribute *bits = sm->new_attribute();
    char *name = sm->new_string("platform_bits");
    bits->set_name(name);
    
    file->add_attribute(bits);
    
    if(elf_class == ELFCLASS32) {
        bits->set_value(32);
        parse_32();
    }
    else if(elf_class == ELFCLASS64) {
        bits->set_value(64);
        parse_64();
    }
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
    
    std::vector<StorageOffset> sections;
    
    StorageAttribute *head = NULL, *last = NULL;
    
    StorageAttribute *shstrtab = NULL;
    
    Elf32_Shdr section;
    for(int s = 0; s < header.e_shnum; s ++) {
        if(read(file_fd, &section, sizeof(section)) != sizeof(section)) {
            status = false;
            return;
        }
        
        StorageAttribute *attribute = sm->new_attribute();
        
        attribute->set_name((const char *)section.sh_name);
        
        sm->create_child(attribute, "size")->set_value(section.sh_size);
        
        sm->create_child(attribute, "offset")->set_value(section.sh_offset);
        sm->create_child(attribute, "address")->set_value(section.sh_addr);
        
        if(last == NULL) head = attribute;
        else last->set_next(attribute);
        last = attribute;
        
        if(s == header.e_shstrndx) shstrtab = attribute;
    }
    
    /* Read the section string table . . . */
    Byte *shstrtab_content = read_content(shstrtab);
    if(shstrtab_content == NULL) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "Could not read content of string table. Perhaps it is a corrupt ELF file?");
        status = false;
        return;
    }
    
    StorageAttribute *attribute = head;
    while(attribute) {
        attribute->set_name(Word(attribute->get_name()) + (char *)shstrtab_content);
        attribute = attribute->get_next();
    }
    
    file->set_sections(head);
    
    /* Parse the symbols now. Note that not every ELF file has static and/or dynamic symbols.
        Some have one, some have both. Some have none, too. */
    /* Try the static symbols . . . */
    StorageAttribute *symtab_offset = file->get_section_attribute(".symtab");
    StorageAttribute *strtab_offset = file->get_section_attribute(".strtab");
    if(symtab_offset != NULL && strtab_offset != NULL) {
        char *symtab_content = (char *)read_content(symtab_offset);
        char *strtab_content = (char *)read_content(strtab_offset);
        
        if(symtab_content == NULL || strtab_content == NULL) {
            status = false;
            return;
        }
        
        Word symtab_size = sm->get_child(symtab_offset, "size")->get_value();
        
        Elf32_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(symtab_size)) {
            memcpy(&symbol, symtab_content + offset, sizeof(symbol));
            
            StorageAttribute *symbol_attribute = sm->new_attribute();
            char *demangled;
            int status;
            
            demangled = abi::__cxa_demangle(strtab_content + symbol.st_name, NULL, NULL, &status);
            if(demangled) {
                symbol_attribute->set_name(sm->new_string(demangled));
                free(demangled);
            }
            else symbol_attribute->set_name(strtab_content + symbol.st_name);
            
            sm->create_child(symbol_attribute, "address")->set_value(symbol.st_value);
            sm->create_child(symbol_attribute, "size")->set_value(symbol.st_size);
            
            file->add_symbol(symbol_attribute);
            
            offset += sizeof(symbol);
        }
    }
    
    /* Now try the dynamic symbols . . . */
    StorageAttribute *dynsym_offset = file->get_section_attribute(".dynsym");
    StorageAttribute *dynstr_offset = file->get_section_attribute(".dynstr");
    if(dynsym_offset != NULL && dynstr_offset != NULL) {
        char *dynsym_content = (char *)read_content(dynsym_offset);
        char *dynstr_content = (char *)read_content(dynstr_offset);
        
        if(dynsym_content == NULL || dynstr_content == NULL) {
            status = false;
            return;
        }
        
        Word dynsym_size = sm->get_child(dynsym_offset, "size")->get_value();
        
        Elf32_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(dynsym_size)) {
            memcpy(&symbol, dynsym_content + offset, sizeof(symbol));
            
            StorageAttribute *symbol_attribute = sm->new_attribute();
            char *demangled;
            int status;
            
            demangled = abi::__cxa_demangle(dynstr_content + symbol.st_name, NULL, NULL, &status);
            if(demangled) {
                symbol_attribute->set_name(sm->new_string(demangled));
                free(demangled);
            }
            else symbol_attribute->set_name(dynstr_content + symbol.st_name);
            
            sm->create_child(symbol_attribute, "address")->set_value(symbol.st_value);
            sm->create_child(symbol_attribute, "size")->set_value(symbol.st_size);
            
            file->add_symbol(symbol_attribute);
            
            offset += sizeof(symbol);
        }
    }
}

void ElfParser::parse_64() {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
    Misc::Message(Misc::Message::WARNING_MESSAGE,
        "Parsing 64-bit elf file, but Aesalon was compiled for 32-bit GNU/Linux. "
        "Please recompile Aesalon for a 64-bit platform and try again.");
    status = false;
    return;
#endif
    StorageManager *sm = file->get_storage_manager();
    /* Read in the header . . . */
    Elf64_Ehdr header;
    if(read(file_fd, &header, sizeof(header)) != sizeof(header)) {
        status = false;
        return;
    }
    
    /* Start off by parsing the sections . . . */
    lseek(file_fd, header.e_shoff, SEEK_SET);
    
    std::vector<StorageOffset> sections;
    
    StorageAttribute *head = NULL, *last = NULL;
    
    StorageAttribute *shstrtab = NULL;
    
    Elf64_Shdr section;
    for(int s = 0; s < header.e_shnum; s ++) {
        if(read(file_fd, &section, sizeof(section)) != sizeof(section)) {
            status = false;
            return;
        }
        
        StorageAttribute *attribute = sm->new_attribute();
        
        attribute->set_name((const char *)section.sh_name);
        
        sm->create_child(attribute, "size")->set_value(section.sh_size);
        
        sm->create_child(attribute, "offset")->set_value(section.sh_offset);
        sm->create_child(attribute, "address")->set_value(section.sh_addr);
        
        if(last == NULL) head = attribute;
        else last->set_next(attribute);
        last = attribute;
        
        if(s == header.e_shstrndx) shstrtab = attribute;
    }
    
    /* Read the section string table . . . */
    Byte *shstrtab_content = read_content(shstrtab);
    if(shstrtab_content == NULL) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "Could not read content of string table. Perhaps it is a corrupt ELF file?");
        status = false;
        return;
    }
    
    StorageAttribute *attribute = head;
    while(attribute) {
        attribute->set_name(Word(attribute->get_name()) + (char *)shstrtab_content);
        attribute = attribute->get_next();
    }
    
    file->set_sections(head);
    
    /* Parse the symbols now. Note that not every ELF file has static and/or dynamic symbols.
        Some have one, some have both. Some have none, too. */
    /* Try the static symbols . . . */
    StorageAttribute *symtab_offset = file->get_section_attribute(".symtab");
    StorageAttribute *strtab_offset = file->get_section_attribute(".strtab");
    if(symtab_offset != NULL && strtab_offset != NULL) {
        char *symtab_content = (char *)read_content(symtab_offset);
        char *strtab_content = (char *)read_content(strtab_offset);
        
        if(symtab_content == NULL || strtab_content == NULL) {
            status = false;
            return;
        }
        
        Word symtab_size = sm->get_child(symtab_offset, "size")->get_value();
        
        Elf64_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(symtab_size)) {
            memcpy(&symbol, symtab_content + offset, sizeof(symbol));
            
            StorageAttribute *symbol_attribute = sm->new_attribute();
            char *demangled;
            int status;
            
            demangled = abi::__cxa_demangle(strtab_content + symbol.st_name, NULL, NULL, &status);
            if(demangled) {
                symbol_attribute->set_name(sm->new_string(demangled));
                free(demangled);
            }
            else symbol_attribute->set_name(strtab_content + symbol.st_name);
            
            sm->create_child(symbol_attribute, "address")->set_value(symbol.st_value);
            sm->create_child(symbol_attribute, "size")->set_value(symbol.st_size);
            
            file->add_symbol(symbol_attribute);
            
            offset += sizeof(symbol);
        }
    }
    
    /* Now try the dynamic symbols . . . */
    StorageAttribute *dynsym_offset = file->get_section_attribute(".dynsym");
    StorageAttribute *dynstr_offset = file->get_section_attribute(".dynstr");
    if(dynsym_offset != NULL && dynstr_offset != NULL) {
        char *dynsym_content = (char *)read_content(dynsym_offset);
        char *dynstr_content = (char *)read_content(dynstr_offset);
        
        if(dynsym_content == NULL || dynstr_content == NULL) {
            status = false;
            return;
        }
        
        Word dynsym_size = sm->get_child(dynsym_offset, "size")->get_value();
        
        Elf64_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(dynsym_size)) {
            memcpy(&symbol, dynsym_content + offset, sizeof(symbol));
            
            StorageAttribute *symbol_attribute = sm->new_attribute();
            char *demangled;
            int status;
            
            demangled = abi::__cxa_demangle(dynstr_content + symbol.st_name, NULL, NULL, &status);
            if(demangled) {
                symbol_attribute->set_name(sm->new_string(demangled));
                free(demangled);
            }
            else symbol_attribute->set_name(dynstr_content + symbol.st_name);
            
            sm->create_child(symbol_attribute, "address")->set_value(symbol.st_value);
            sm->create_child(symbol_attribute, "size")->set_value(symbol.st_size);
            
            file->add_symbol(symbol_attribute);
            
            offset += sizeof(symbol);
        }
    }
}

Byte *ElfParser::read_content(StorageAttribute *section) {
    StorageManager *sm = file->get_storage_manager();
    
    StorageAttribute *data = sm->get_child(section, "data");
    
    if(data != NULL) return (Byte *)data->get_value();
    
    Word size = sm->get_child(section, "size")->get_value();
    Byte *read_data = (Byte *)sm->new_string(size);
    
    lseek(file_fd, sm->get_child(section, "offset")->get_value(), SEEK_SET);
    if(read(file_fd, read_data, size) != (SWord)size) return 0;
    
    sm->create_child(section, "data")->set_value((Word)read_data);
    
    return read_data;
}

} // namespace Analyzer

/*#endif*/
