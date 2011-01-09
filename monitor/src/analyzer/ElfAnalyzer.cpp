/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/analyzer/ElfAnalyzer.cpp

*/

#include <stdio.h>
#include <fcntl.h>
#include <elf.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "analyzer/ElfAnalyzer.h"
#include "config/ConcreteVault.h"
#include "common/PathSanitizer.h"
#include "common/StreamAsString.h"

namespace Monitor {
namespace Analyzer {

ElfAnalyzer::ElfAnalyzer(std::string filename) : ExecutableAnalyzer(filename), m_vault(NULL) {
	filename = Common::PathSanitizer::sanitize(filename);
	m_fd = open(filename.c_str(), O_RDONLY);
	if(readIdent()) {
		m_vault = new Config::ConcreteVault();
		if(m_elfType == ELF32) {
			parseElf<Elf32_Word, Elf32_Ehdr, Elf32_Shdr, Elf32_Sym>();
		}
		else if(m_elfType == ELF64) {
			parseElf<Elf64_Word, Elf64_Ehdr, Elf64_Shdr, Elf64_Sym>();
		}
	}
	close(m_fd);
}

ElfAnalyzer::~ElfAnalyzer() {
	
}

bool ElfAnalyzer::readIdent() {
	unsigned char ident[EI_NIDENT];
	if(read(m_fd, ident, sizeof(ident)) != EI_NIDENT) return false;
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

template<typename ElfWord, typename ELFHeader, typename SectionHeader, typename SymbolHeader>
void ElfAnalyzer::parseElf() {
	if(m_encoding != LSB_ENCODING) {
		std::cout << "Don't know how to handle non-LSB encoding at the moment." << std::endl;
		return;
	}
	
	lseek(m_fd, 0, SEEK_SET);
	ELFHeader eheader;
	if(read(m_fd, &eheader, sizeof(eheader)) != sizeof(eheader)) return;
	
	std::vector<SectionHeader> sectionList;
	
	/* Reserve 16 sections to start with. */
	sectionList.reserve(16);
	
	SectionHeader sheader;
	lseek(m_fd, eheader.e_shoff, SEEK_SET);
	for(int shnum = 0; shnum < eheader.e_shnum; shnum ++) {
		if(read(m_fd, &sheader, sizeof(sheader)) != sizeof(sheader)) {
			return;
		}
		sectionList.push_back(sheader);
	}
	
	
}

} // namespace Analyzer
} // namespace Monitor
