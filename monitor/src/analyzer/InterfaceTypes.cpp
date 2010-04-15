#include "InterfaceTypes.h"

namespace Analyzer {

Object::Object(const std::string &name, Word address, Word size) : name(name), address(address), size(size) {

}

Object::~Object() {

}

} // namespace Analyzer
