/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/analyzer/ElfAnalyzer.h

*/

#ifndef AesalonMonitor_Analyzer_ElfAnalyzer_H
#define AesalonMonitor_Analyzer_ElfAnalyzer_H

#include <stdint.h>

#include "ExecutableAnalyzer.h"

#define ELF32_TYPES Elf32_Ehdr, Elf32_Shdr, Elf32_Sym
#define ELF64_TYPES Elf64_Ehdr, Elf64_Shdr, Elf64_Sym

namespace Monitor {
namespace Analyzer {

class ElfAnalyzer : public ExecutableAnalyzer {
private:
	int m_fd;
	Config::Vault *m_vault;
	uint8_t *m_file;
	uint32_t m_fileSize;
	
	enum ElfType {
		ELF32,
		ELF64
	} m_elfType;
	
	enum Encoding {
		MSB_ENCODING,
		LSB_ENCODING
	} m_encoding;
public:
	ElfAnalyzer(std::string filename);
	virtual ~ElfAnalyzer();
	
	virtual Config::Vault *analyzerVault() { return m_vault; }
private:
	bool identValid();
	
	template<typename ELFHeader, typename SectionHeader, typename SymbolHeader>
	void parseElf();
	
	template<typename SymbolHeader>
	void parseSymbols(SymbolHeader *symbols, int symbolCount, const char *stringTable);
};

} // namespace Analyzer
} // namespace Monitor

#endif
