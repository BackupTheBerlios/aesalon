#include "Symbol.h"

namespace Program {

Symbol::Symbol(Address address) : m_address(address) {

}

Symbol::~Symbol() {

}

bool Symbol::operator<(const Program::Symbol &other) const {
	return m_address < other.m_address;
}


} // namespace Program
