#include <dlfcn.h>

#include "Module.h"

namespace Module {

Module::Module(std::string moduleName) : m_moduleName(moduleName) {
	
}

Module::~Module() {
	
}

} // namespace Module
