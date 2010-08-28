#ifndef Configuration_H
#define Configuration_H

#include <map>
#include <vector>

#include "ConfigurationModule.h"

namespace Misc {

class Configuration {
public:
	Configuration(char **argv);
	~Configuration();
private:
	typedef std::map<std::string, ConfigurationModule *> ModuleMap;
	ModuleMap m_moduleMap;
	std::vector<std::string> m_launchArguments;
	char **m_argv;
public:
	ConfigurationModule *module(const std::string &name);
	const std::vector<std::string> &launchArguments() const { return m_launchArguments; }
	
	void processFile(const std::string &path);
	void processSearchPaths();
	void processArgv();
	
	ConfigurationItem *traverse(std::string path);
private:
	void addLaunchArgument(const std::string &argument);
};

} // namespace Misc

#endif
