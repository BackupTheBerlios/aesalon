#ifndef Configuration_H
#define Configuration_H

#include <map>

#include "ConfigurationModule.h"

namespace Misc {

class Configuration {
public:
	Configuration();
	~Configuration();
private:
	typedef std::map<std::string, ConfigurationModule *> ModuleMap;
	ModuleMap m_moduleMap;
	std::string m_launchArguments;
public:
	ConfigurationModule *module(const std::string &name);
	
	void processFile(const std::string &path);
	void processArgv(const char **argv);
	void copyArgv();
private:
	void addLaunchArgument(const std::string &argument);
};

} // namespace Misc

#endif
