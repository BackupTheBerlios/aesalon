#ifndef Configuration_H
#define Configuration_H

#include <string>
#include <map>

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
public:
	Configuration *singleton() const { return m_singleton; }
	const ConfigItems &configItems() const { return m_configItems; }
private:
	void addConfigItems();
	void processConfigFiles();
	void processArguments();
};

} // namespace Misc

#endif
