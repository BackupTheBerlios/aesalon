#ifndef AESALON_CONFIG_PARSER_H
#define AESALON_CONFIG_PARSER_H

#include <map>
#include <string>

namespace Misc {

class ConfigParser {
private:
    std::map<std::string, std::string> config_map;
    void parse_config_file(std::string filename);
public:
    ConfigParser();
    virtual ~ConfigParser();
    
    std::string get_config_item(std::string reference_name) {
        return config_map[reference_name];
    }
};

} // namespace Misc

#endif
