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
    
    /* Set platform "bits" attribute */
    StorageManager *sm = file->get_storage_manager();
    StorageOffset bits_offset = sm->new_attribute();
    StorageOffset string = sm->new_string("platform_bits");
    sm->dereference_attribute(bits_offset)->name = string;
    
    file->add_attribute(bits_offset);
    
    if(elf_class == ELFCLASS32) {
        sm->dereference_attribute(bits_offset)->value = 32;
        parse_32();
    }
    else if(elf_class == ELFCLASS64) {
        sm->dereference_attribute(bits_offset)->value = 64;
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
    
    StorageOffset head = -1;
    StorageOffset last = -1;
    
    StorageOffset shstrtab = -1;
    
    Elf32_Shdr section;
    for(int s = 0; s < header.e_shnum; s ++) {
        if(read(file_fd, &section, sizeof(section)) != sizeof(section)) {
            status = false;
            return;
        }
        
        StorageOffset item_offset = sm->new_attribute();
        
        sm->dereference_attribute(item_offset)->name = section.sh_name;
        
        StorageAttribute *size = sm->create_child(item_offset, "size");
        size->value = section.sh_size;
        
        sm->create_child(item_offset, "offset")->value = section.sh_offset;
        sm->create_child(item_offset, "address")->value = section.sh_addr;
        
        if(last == -1) head = item_offset;
        else sm->dereference_attribute(last)->next = item_offset;
        last = item_offset;
        
        if(s == header.e_shstrndx) shstrtab = item_offset;
    }
    
    /* Read the section string table . . . */
    StorageOffset shstrtab_content = read_content(shstrtab);
    if(shstrtab_content == 0) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "Could not read content of string table. Perhaps it is a corrupt ELF file?");
        status = false;
        return;
    }
    
    StorageOffset offset = head;
    while(offset != -1) {
        StorageAttribute *attribute = sm->dereference_attribute(offset);
        attribute->name += shstrtab_content;
        offset = attribute->next;
    }
    
    file->set_sections(head);
    
    /* Parse the symbols now. Note that not every ELF file has static and/or dynamic symbols.
        Some have one, some have both. Some have none, too. */
    /* Try the static symbols . . . */
    StorageOffset symtab_offset = file->get_section_offset(".symtab");
    StorageOffset strtab_offset = file->get_section_offset(".strtab");
    if(symtab_offset != -1 && strtab_offset != -1) {
        StorageOffset symtab_content_offset = read_content(symtab_offset);
        StorageOffset strtab_content_offset = read_content(strtab_offset);
        
        if(symtab_content_offset == -1 || strtab_content_offset == -1) {
            status = false;
            return;
        }
        
        Word symtab_size = sm->dereference_attribute(sm->get_child(symtab_offset, "size"))->value;
        
        Elf32_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(symtab_size)) {
            memcpy(&symbol, sm->dereference_string(symtab_content_offset) + offset, sizeof(symbol));
            
            StorageOffset symbol_offset = sm->new_attribute();
            sm->dereference_attribute(symbol_offset)->name = strtab_content_offset + symbol.st_name;
            sm->create_child(symbol_offset, "address")->value = symbol.st_value;
            sm->create_child(symbol_offset, "size")->value = symbol.st_size;
            
            file->add_symbol(symbol_offset);
            
            offset += sizeof(symbol);
        }
    }
    
    /* Now try the dynamic symbols . . . */
    StorageOffset dynsym_offset = file->get_section_offset(".dynsym");
    StorageOffset dynstr_offset = file->get_section_offset(".dynstr");
    if(dynsym_offset != -1 && dynstr_offset != -1) {
        StorageOffset dynsym_content_offset = read_content(dynsym_offset);
        StorageOffset dynstr_content_offset = read_content(dynstr_offset);
        
        if(dynsym_content_offset == -1 || dynstr_content_offset == -1) {
            status = false;
            return;
        }
        
        Word dynsym_size = sm->dereference_attribute(sm->get_child(dynsym_offset, "size"))->value;
        
        Elf32_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(dynsym_size)) {
            memcpy(&symbol, sm->dereference_string(dynsym_content_offset) + offset, sizeof(symbol));
            
            StorageOffset symbol_offset = sm->new_attribute();
            sm->dereference_attribute(symbol_offset)->name = dynstr_content_offset + symbol.st_name;
            sm->create_child(symbol_offset, "address")->value = symbol.st_value;
            sm->create_child(symbol_offset, "size")->value = symbol.st_size;
            
            file->add_symbol(symbol_offset);
            
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
    
    StorageOffset head = -1;
    StorageOffset last = -1;
    
    StorageOffset shstrtab = -1;
    
    Elf64_Shdr section;
    for(int s = 0; s < header.e_shnum; s ++) {
        if(read(file_fd, &section, sizeof(section)) != sizeof(section)) {
            status = false;
            return;
        }
        
        StorageOffset item_offset = sm->new_attribute();
        
        sm->dereference_attribute(item_offset)->name = section.sh_name;
        
        StorageAttribute *size = sm->create_child(item_offset, "size");
        size->value = section.sh_size;
        
        sm->create_child(item_offset, "offset")->value = section.sh_offset;
        sm->create_child(item_offset, "address")->value = section.sh_addr;
        
        if(last == -1) head = item_offset;
        else sm->dereference_attribute(last)->next = item_offset;
        last = item_offset;
        
        if(s == header.e_shstrndx) shstrtab = item_offset;
    }
    
    /* Read the section string table . . . */
    StorageOffset shstrtab_content = read_content(shstrtab);
    if(shstrtab_content == 0) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "Could not read content of string table. Perhaps it is a corrupt ELF file?");
        status = false;
        return;
    }
    
    StorageOffset offset = head;
    while(offset != -1) {
        StorageAttribute *attribute = sm->dereference_attribute(offset);
        attribute->name += shstrtab_content;
        offset = attribute->next;
    }
    
    file->set_sections(head);
    
    /* Parse the symbols now. Note that not every ELF file has static and/or dynamic symbols.
        Some have one, some have both. Some have none, too. */
    /* Try the static symbols . . . */
    StorageOffset symtab_offset = file->get_section_offset(".symtab");
    StorageOffset strtab_offset = file->get_section_offset(".strtab");
    if(symtab_offset != -1 && strtab_offset != -1) {
        StorageOffset symtab_content_offset = read_content(symtab_offset);
        StorageOffset strtab_content_offset = read_content(strtab_offset);
        
        if(symtab_content_offset == -1 || strtab_content_offset == -1) {
            status = false;
            return;
        }
        
        Word symtab_size = sm->dereference_attribute(sm->get_child(symtab_offset, "size"))->value;
        
        Elf64_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(symtab_size)) {
            memcpy(&symbol, sm->dereference_string(symtab_content_offset) + offset, sizeof(symbol));
            
            StorageOffset symbol_offset = sm->new_attribute();
            sm->dereference_attribute(symbol_offset)->name = strtab_content_offset + symbol.st_name;
            sm->create_child(symbol_offset, "address")->value = symbol.st_value;
            sm->create_child(symbol_offset, "size")->value = symbol.st_size;
            
            file->add_symbol(symbol_offset);
            
            offset += sizeof(symbol);
        }
    }
    
    /* Now try the dynamic symbols . . . */
    StorageOffset dynsym_offset = file->get_section_offset(".dynsym");
    StorageOffset dynstr_offset = file->get_section_offset(".dynstr");
    if(dynsym_offset != -1 && dynstr_offset != -1) {
        StorageOffset dynsym_content_offset = read_content(dynsym_offset);
        StorageOffset dynstr_content_offset = read_content(dynstr_offset);
        
        if(dynsym_content_offset == -1 || dynstr_content_offset == -1) {
            status = false;
            return;
        }
        
        Word dynsym_size = sm->dereference_attribute(sm->get_child(dynsym_offset, "size"))->value;
        
        Elf64_Sym symbol;
        StorageOffset offset = 0;
        while(offset < SWord(dynsym_size)) {
            memcpy(&symbol, sm->dereference_string(dynsym_content_offset) + offset, sizeof(symbol));
            
            StorageOffset symbol_offset = sm->new_attribute();
            sm->dereference_attribute(symbol_offset)->name = dynstr_content_offset + symbol.st_name;
            sm->create_child(symbol_offset, "address")->value = symbol.st_value;
            sm->create_child(symbol_offset, "size")->value = symbol.st_size;
            
            file->add_symbol(symbol_offset);
            
            offset += sizeof(symbol);
        }
    }
}

StorageOffset ElfParser::read_content(StorageOffset section) {
    StorageManager *sm = file->get_storage_manager();
    
    StorageOffset data_offset = sm->get_child(section, "data");
    
    if(data_offset != -1) return sm->dereference_attribute(data_offset)->value;
        
    StorageOffset size_offset = sm->get_child(section, "size");
    Word data_size = sm->dereference_attribute(size_offset)->value;
    StorageOffset read_data_offset = sm->new_string(data_size);
    
    lseek(file_fd, sm->dereference_attribute(sm->get_child(section, "offset"))->value, SEEK_SET);
    if(read(file_fd, sm->dereference_string(read_data_offset), data_size) != (SWord)data_size) return 0;
    
    sm->create_child(section, "data")->value = read_data_offset;
    
    return read_data_offset;
}

} // namespace Analyzer

/*#endif*/
