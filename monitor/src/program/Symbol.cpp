#include <iostream>

#include "Symbol.h"

namespace Program {

Symbol::Symbol(Address address, Address size, const char *name) : m_address(address), m_size(size), m_name(name) {
	/*std::cout << "Symbol(" << address << ", " << size << ", \"" << name << "\") . . ." << std::endl;*/
}

Symbol::~Symbol() {

}

bool Symbol::operator<(const Program::Symbol &other) const {
	return m_address < other.m_address;
}


} // namespace Program
