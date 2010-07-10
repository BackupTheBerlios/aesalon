#ifndef ElfAnalyzer_H
#define ElfAnalyzer_H

#include "Analyzer.h"

namespace Program {

class ElfAnalyzer : public Analyzer {
public:
	ElfAnalyzer(std::string filename);
	virtual ~ElfAnalyzer();
private:
	int m_fd;
public:
	int fd() const { return m_fd; }
private:
	void parse();
};

} // namespace Program

#endif
