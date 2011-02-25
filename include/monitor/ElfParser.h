/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/ElfParser.h
*/

#ifndef AesalonMonitor_ElfParser_H
#define AesalonMonitor_ElfParser_H

#include <stdint.h>
#include <string>

#include "config/Vault.h"

#define ELF32_TYPES Elf32_Ehdr, Elf32_Shdr, Elf32_Sym
#define ELF64_TYPES Elf64_Ehdr, Elf64_Shdr, Elf64_Sym

namespace Monitor {

class ElfParser {
public:
	class Processor {
	public:
		virtual ~Processor() {}
		
		virtual void process(const char *symbolName, uint64_t symbolAddress, uint64_t symbolSize) = 0;
	};
	int m_fd;
	uint8_t *m_file;
	uint32_t m_fileSize;
	Processor *m_processor;
	
	enum ElfType {
		ELF32,
		ELF64
	} m_elfType;
	
	enum Encoding {
		MSB_ENCODING,
		LSB_ENCODING
	} m_encoding;
public:
	ElfParser();
	virtual ~ElfParser();
	
	void parse(const std::string &filename, Processor *processor);
private:
	bool identValid();
	
	template<typename ELFHeader, typename SectionHeader, typename SymbolHeader>
	void parseElf();
	
	template<typename SymbolHeader>
	void parseSymbols(SymbolHeader *symbols, int symbolCount, const char *stringTable);
};

} // namespace Monitor

#endif
