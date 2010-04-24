#include "InterfaceTypes.h"

namespace Analyzer {

Object::Object(const std::string &name, Word address, Word size) : name(name), address(address), size(size) {

}

Object::~Object() {

}

bool Object::operator<(const Analyzer::Object &other) const {
    return address < other.address;
}

bool Object::operator<(Word address) const {
    return this->address < address;
}

bool Object::operator>(Word address) const {
    return this->address > address;
}

} // namespace Analyzer
