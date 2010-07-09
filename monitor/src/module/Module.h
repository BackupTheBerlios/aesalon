#ifndef Module_H
#define Module_H

#include <string>

#include "DataTypes.h"

namespace Module {

class Module {
public:
	Module(std::string moduleName);
	~Module();
private:
	uint16_t m_moduleID;
	std::string m_moduleName;
	void *m_monitorHandle;
public:
	uint16_t moduleID() const { return m_moduleID; }
	const std::string &moduleName() const { return m_moduleName; }
	
};

} // namespace Module

#endif
