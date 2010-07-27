#ifndef Configuration_H
#define Configuration_H

#include <string>
#include <map>
#include <vector>

#include "ConfigurationItem.h"

namespace Misc {

class Configuration {
public:
	Configuration(char *argv[]);
	~Configuration();
	
	typedef std::map<std::string, ConfigurationItem *> ConfigItems;
private:
	char **m_argv;
	static Configuration *m_singleton;
	ConfigItems m_configItems;
	std::string m_filename;
	std::vector<std::string> m_programArguments;
public:
	Configuration *singleton() const { return m_singleton; }
	ConfigItems &configItems() { return m_configItems; }
	const ConfigItems &configItems() const { return m_configItems; }
	const std::string &filename() const { return m_filename; }
	const std::vector<std::string> &programArguments() const { return m_programArguments; }
	
	void addConfigItem(ConfigurationItem *item);
	void processConfigFile(std::string path);
private:
	void addConfigItems();
	void processDefaultConfigFiles();
	void processArguments();
	void processItem(std::string itemStr);
};

} // namespace Misc

#endif
