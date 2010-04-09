#include "InterfaceTypes.h"

namespace Analyzer {

Symbol::Symbol(const std::string &name, Word address, Word size) : name(name), address(address), size(size) {

}

Symbol::~Symbol() {

}

} // namespace Analyzer
