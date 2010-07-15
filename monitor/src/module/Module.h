#ifndef Module_H
#define Module_H

#include <string>

#include "DataTypes.h"
#include "interface/Interface.h"

namespace Module {

class Module {
public:
	Module(uint16_t moduleID, std::string moduleName);
	~Module();
private:
	uint16_t m_moduleID;
	std::string m_moduleName;
	void *m_monitorHandle;
	MonitorInterface *m_interface;
public:
	uint16_t moduleID() const { return m_moduleID; }
	const std::string &moduleName() const { return m_moduleName; }
	MonitorInterface *interface() const { return m_interface; }
	
	DataPacket *processPacket(DataPacket *packet);
};

} // namespace Module

#endif
