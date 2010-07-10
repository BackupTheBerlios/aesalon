#include <fcntl.h>
#include <errno.h>
#include <linux/elf.h>
#include <string.h>

#include "ElfAnalyzer.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"

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
	ELFCLASS32) {
#elif defined(AesalonPlatform_x86_64)
	ELFCLASS64) {
#endif
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
	}
}

} // namespace Program
