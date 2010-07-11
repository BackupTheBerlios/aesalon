#include <algorithm>
#include <string.h>
#include <iostream>

#include "Analyzer.h"

namespace Program {

static bool compareSymbols(Symbol *one, Symbol *two) {
	if(one == NULL || two == NULL) return false;
	return *one < *two;
}

Analyzer::Analyzer(std::string filename) : m_filename(filename) {
	
}

Analyzer::~Analyzer() {
	for(SectionList::iterator i = m_sectionList.begin(); i != m_sectionList.end(); i ++) {
		delete (*i);
	}
	
	for(SymbolList::iterator i = m_symbolList.begin(); i != m_symbolList.end(); i ++) {
		delete (*i);
	}
}

Symbol *Analyzer::symbol(Address address) const {
	Symbol s(address, 0, 0);
	return *std::lower_bound(m_symbolList.begin(), m_symbolList.end(), &s, compareSymbols);
}

Section *Analyzer::section(int index) const {
	return m_sectionList[index];
}

Section *Analyzer::section(const char *name) const {
	for(SectionList::const_iterator i = m_sectionList.begin(); i != m_sectionList.end(); i ++) {
		if(!strcmp(name, (*i)->name())) return *i;
	}
	return NULL;
}

void Analyzer::addSymbol(Symbol *symbol) {
	m_symbolList.insert(std::lower_bound(m_symbolList.begin(), m_symbolList.end(), symbol, compareSymbols), symbol);
	
}

void Analyzer::addSection(Section *section) {
	m_sectionList.push_back(section);
}

void Analyzer::sortSymbolList() {
	std::sort(m_symbolList.begin(), m_symbolList.end());
}

} // namespace Program
