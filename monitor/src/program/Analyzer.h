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
	std::vector<Symbol *> m_symbolList;
public:
	const std::string &filename() const { return m_filename; }
	
	Symbol *symbol(Address address) const;
protected:
	void addSymbol(Symbol *symbol);
	void sortSymbolList();
};

} // namespace Program

#endif
