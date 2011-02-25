/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/ElfParser.cpp
*/

#include <stdio.h>
#include <fcntl.h>
#include <elf.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sys/mman.h>

#include "monitor/ElfParser.h"
#include "util/PathSanitizer.h"
#include "util/StreamAsString.h"

namespace Monitor {

ElfParser::ElfParser() {
	
}

ElfParser::~ElfParser() {
	
}

void ElfParser::parse(const std::string &filename, Processor *processor) {
	m_processor = processor;
	m_fd = open(filename.c_str(), O_RDONLY);
	
	m_fileSize = lseek(m_fd, 0, SEEK_END);
	m_file = reinterpret_cast<uint8_t *>(mmap(NULL, m_fileSize, PROT_READ, MAP_PRIVATE, m_fd, 0));
	
	if(identValid()) {
		if(m_elfType == ELF32) {
			parseElf<ELF32_TYPES>();
		}
		else if(m_elfType == ELF64) {
			parseElf<ELF64_TYPES>();
		}
	}
	close(m_fd);
	munmap(m_file, m_fileSize);
}

bool ElfParser::identValid() {
	if(m_fileSize < EI_NIDENT) return false;
	unsigned char *ident = reinterpret_cast<unsigned char *>(m_file);
	if(strncmp(ELFMAG, (char *)ident, SELFMAG) != 0) return false;
	
	/* By this point, it's probably valid. */
	if(ident[EI_CLASS] == ELFCLASS32) m_elfType = ELF32;
	else if(ident[EI_CLASS] == ELFCLASS64) m_elfType = ELF64;
	/* But just in case . . . */
	else return false;
	
	if(ident[EI_DATA] == ELFDATA2LSB) m_encoding = LSB_ENCODING;
	else if(ident[EI_DATA] == ELFDATA2MSB) m_encoding = MSB_ENCODING;
	/* Only know how to handle MSB and LSB encoding . . . */
	else return false;
	
	/* If the execution has gotten this far, then it's a likely bet that
		it's a valid ELF file.
	*/
	return true;
}

template<typename ELFHeader, typename SectionHeader, typename SymbolHeader>
void ElfParser::parseElf() {
	if(m_encoding != LSB_ENCODING) {
		std::cout << "Don't know how to handle non-LSB encoding at the moment." << std::endl;
		return;
	}
	
	lseek(m_fd, 0, SEEK_SET);
	ELFHeader *eheader;
	eheader = reinterpret_cast<ELFHeader *>(m_file);
	
	SectionHeader *sections = reinterpret_cast<SectionHeader *>(m_file + eheader->e_shoff);
	
	SectionHeader *shstrSection = sections + eheader->e_shstrndx;
	const char *shstr = reinterpret_cast<char *>(m_file + shstrSection->sh_offset);
	
	SymbolHeader *symtab = NULL;
	int symtabSize = 0;
	const char *strtab = NULL;
	SymbolHeader *dynsym = NULL;
	int dynsymSize = 0;
	const char *dynstr = NULL;
	
	for(int i = 0; i < eheader->e_shnum; i ++) {
		SectionHeader *section = &sections[i];
		if(!strcmp(".symtab", shstr + section->sh_name)) {
			symtab = reinterpret_cast<SymbolHeader *>(m_file + section->sh_offset);
			symtabSize = section->sh_size / sizeof(SymbolHeader);
		}
		else if(!strcmp(".strtab", shstr + section->sh_name)) {
			strtab = reinterpret_cast<char *>(m_file + section->sh_offset);
		}
		else if(!strcmp(".dynsym", shstr + section->sh_name)) {
			dynsym = reinterpret_cast<SymbolHeader *>(m_file + section->sh_offset);
			dynsymSize = section->sh_size / sizeof(SymbolHeader);
		}
		else if(!strcmp(".dynstr", shstr + section->sh_name)) {
			dynstr = reinterpret_cast<char *>(m_file + section->sh_offset);
		}
	}
	
	parseSymbols<SymbolHeader>(symtab, symtabSize, strtab);
	parseSymbols<SymbolHeader>(dynsym, dynsymSize, dynstr);
}

template<typename SymbolHeader>
void ElfParser::parseSymbols(SymbolHeader *symbols, int symbolCount, const char *stringTable) {
	for(int i = 0; i < symbolCount; i ++) {
		m_processor->process(stringTable + symbols[i].st_name, symbols[i].st_value, symbols[i].st_size);
		/*std::cout << "Parsing symbol \"" << stringTable + symbols[i].st_name << "\" . . .\n";*/
		/*m_vault->set(
			Util::StreamAsString() << "\"" << stringTable + symbols[i].st_name << "\":value",
			Util::StreamAsString() << symbols[i].st_value);
		m_vault->set(
			Util::StreamAsString() << "\"" << stringTable + symbols[i].st_name << "\":size",
			Util::StreamAsString() << symbols[i].st_size);*/
	}
}

} // namespace Monitor
