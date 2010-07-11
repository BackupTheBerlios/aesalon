#ifndef Analyzer_H
#define Analyzer_H

#include <string>
#include <vector>

#include "Section.h"
#include "Symbol.h"

namespace Program {

class Analyzer {
public:
	Analyzer(std::string filename);
	~Analyzer();
private:
	std::string m_filename;
	typedef std::vector<Symbol *> SymbolList;
	SymbolList m_symbolList;
	typedef std::vector<Section *> SectionList;
	SectionList m_sectionList;
public:
	const std::string &filename() const { return m_filename; }
	
	Symbol *symbol(Address address) const;
	Section *section(int index) const;
	Section *section(const char *name) const;
protected:
	void addSymbol(Symbol *symbol);
	void addSection(Section *section);
	void sortSymbolList();
};

} // namespace Program

#endif
