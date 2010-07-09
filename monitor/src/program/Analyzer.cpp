#include <algorithm>

#include "Analyzer.h"

namespace Program {

static bool compareSymbols(Symbol *one, Symbol *two) {
	if(one == NULL || two == NULL) return false;
	return *one < *two;
}

Analyzer::Analyzer(std::string filename) : m_filename(filename) {

}

Analyzer::~Analyzer() {

}

Symbol *Analyzer::symbol(Address address) const {
	Symbol s(address);
	return *std::lower_bound(m_symbolList.begin(), m_symbolList.end(), &s, compareSymbols);
}

void Analyzer::addSymbol(Symbol *symbol) {
	m_symbolList.insert(std::lower_bound(m_symbolList.begin(), m_symbolList.end(), symbol, compareSymbols), symbol);
	
}

void Analyzer::sortSymbolList() {
	std::sort(m_symbolList.begin(), m_symbolList.end());
}

} // namespace Program
