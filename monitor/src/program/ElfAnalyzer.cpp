#include <fcntl.h>
#include <errno.h>
#include <linux/elf.h>
#include <string.h>
#include <iostream>

#include "ElfAnalyzer.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"
#include "Section.h"

namespace Program {

ElfAnalyzer::ElfAnalyzer(std::string filename): Analyzer(filename) {
	m_fd = open(filename.c_str(), O_RDONLY);
	if(m_fd == -1) {
		LogSystem::logAnalyzerMessage(this, Misc::StreamAsString() << "Couldn't open file for analysis: " << strerror(errno));
		return;
	}
	parse();
}

ElfAnalyzer::~ElfAnalyzer() {
	close(m_fd);
}

void ElfAnalyzer::parse() {
	/* Read in the ELF header. */
	Elf64_Ehdr header;
	int ret = read(m_fd, &header, sizeof(header));
	if(ret != sizeof(header)) {
		LogSystem::logAnalyzerMessage(this, Misc::StreamAsString() << "Couldn't read ELF header: " << strerror(errno));
		return;
	}
	
	if(strncmp((char *)header.e_ident, ELFMAG, 4)) {
		LogSystem::logAnalyzerMessage(this, "ELF header is incorrect.");
		return;
	}
	
	if(header.e_ident[EI_CLASS] !=
#ifdef AesalonPlatform_x86
	ELFCLASS32
#elif defined(AesalonPlatform_x86_64)
	ELFCLASS64
#endif
) {
		LogSystem::logAnalyzerMessage(this, "Wrong ELFCLASS detected.");
		return;
	}
	
	Elf64_Shdr section;
	lseek(m_fd, header.e_shoff, SEEK_SET);
	for(int i = 0; i < header.e_shnum; i ++) {
		ret = read(m_fd, &section, sizeof(section));
		if(ret != sizeof(section)) {
			LogSystem::logAnalyzerMessage(this, Misc::StreamAsString() << "Couldn't read section header: " << strerror(errno));
			return;
		}
		
		addSection(new Section(section.sh_addr, section.sh_size, section.sh_offset, section.sh_name));
	}
	
	Section *shstr = this->section(header.e_shstrndx);
	
	if(!readSection(shstr)) return;
	
	for(int i = 0; i < header.e_shnum; i ++) {
		Section *section = this->section(i);
		section->setName((const char *)shstr->data() + section->nameOffset());
	}
	
	parseSymbols(this->section(".symtab"), this->section(".strtab"));
	parseSymbols(this->section(".dynsym"), this->section(".dynstr"));
}

uint8_t *ElfAnalyzer::readSection(Section *section) {
	if(section->data()) return section->data();
	uint8_t *data = new uint8_t[section->size()];
	lseek(m_fd, section->fileOffset(), SEEK_SET);
	int ret = read(m_fd, data, section->size());
	if(ret != (int)section->size()) {
		LogSystem::logAnalyzerMessage(this, Misc::StreamAsString() << "Couldn't read section content: " << strerror(errno));
		return NULL;
	}
	section->setData(data);
	return data;
}

void ElfAnalyzer::parseSymbols(Section *symbolTable, Section *stringTable) {
	if(symbolTable && stringTable && readSection(symbolTable) && readSection(stringTable)) {
		Elf64_Sym symbol;
		Address offset = 0;
		while(offset < symbolTable->size()) {
			memcpy(&symbol, symbolTable->data() + offset, sizeof(symbol));
			offset += sizeof(symbol);
			
			addSymbol(new Symbol(symbol.st_value, symbol.st_size, ((const char *)stringTable->data() + symbol.st_name)));
		}
	}
}

} // namespace Program
