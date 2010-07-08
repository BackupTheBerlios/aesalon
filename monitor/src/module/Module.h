#ifndef Module_H
#define Module_H

#include <string>

namespace Module {

class Module {
public:
	Module(std::string moduleName);
	~Module();
private:
	std::string m_moduleName;
	void *m_monitorHandle;
public:
	const std::string &moduleName() const { return m_moduleName; }
};

} // namespace Module

#endif
