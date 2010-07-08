#ifndef Configuration_H
#define Configuration_H

#include <string>
#include <map>

namespace Misc {

class Configuration {
public:
	Configuration(char *argv[]);
	~Configuration();
	
	typedef std::map<std::string, std::string> ConfigItems;
private:
	static Configuration *m_singleton;
	ConfigItems m_configItems;
public:
	Configuration *singleton() const { return m_singleton; }
	const ConfigItems &configItems() const { return m_configItems; }
};

} // namespace Misc

#endif
