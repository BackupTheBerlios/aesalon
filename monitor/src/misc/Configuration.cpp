#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "Configuration.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"

namespace Misc {

Configuration::Configuration(char **argv) : m_argv(argv) {
	processSearchPaths();
	processFile(AesalonGlobalConfig);
	processFile(AesalonUserConfig);
	processFile(AesalonLocalConfig);
	processArgv();
}

Configuration::~Configuration() {
	for(ModuleMap::iterator i = m_moduleMap.begin(); i != m_moduleMap.end(); i ++) {
		if(i->second != NULL) delete i->second;
	}
}

ConfigurationModule *Configuration::module(const std::string &name) {
	ConfigurationModule *module = m_moduleMap[name];
	if(module == NULL) {
		module = new ConfigurationModule(name);
		m_moduleMap[name] = module;
	}
	return module;
}

void Configuration::processFile(const std::string &path) {
	std::ifstream file(path.c_str(), std::ios_base::in);
	
	if(!file.is_open()) return;
	
	ConfigurationModule *module = this->module("global");
	
	while(!file.eof()) {
		char buffer[1024];
		file.getline(buffer, sizeof(buffer));
		
		if(buffer[0] == '#') continue;
		
		std::string line = buffer;
		if(line.length() == 0) continue;
		
		std::string::size_type divider = line.find(' ');
		std::string command = line.substr(0, divider);
		std::string content;
		
		if(divider == std::string::npos) content = "";
		else content = line.substr(divider+1);
		
		if(command == "module") {
			module = this->module(content);
		}
		else if(command == "set") {
			divider = content.find('=');
			if(divider == std::string::npos) {
				LogSystem::logConfigurationMessage(Misc::StreamAsString() << "Mangled configuration file: set has no argument.");
			}
			std::string variable = content.substr(0, divider);
			content.erase(0, divider+1);
			module->item(variable)->setData(content);
		}
		else {
			LogSystem::logConfigurationMessage(Misc::StreamAsString() << "Mangled configuration file: unknown command \"" << command << "\"");
		}
	}
	
	file.close();
}

void Configuration::processSearchPaths() {
	const char *pathEnv = getenv("AesalonSearchPath");
	if(pathEnv == NULL) {
		LogSystem::logConfigurationMessage("Enviroment variable AesalonSearchPath not set; cannot load modules.");
		return;
	}
	std::string searchPaths = pathEnv;
	
	traverse("search-paths")->setData(searchPaths);
	
	do {
		std::string::size_type position = searchPaths.find(":");
		std::string path = searchPaths.substr(0, position);
		if(position != std::string::npos) searchPaths.erase(0, searchPaths.find(":") + 1);
		else searchPaths.erase();
		
		DIR *directory = opendir(path.c_str());
		if(directory == NULL) {
			LogSystem::logConfigurationMessage(Misc::StreamAsString() << "Cannot open search path \"" << path << "\": " << strerror(errno));
		}
		else {
			struct dirent *file;
			
			while((file = readdir(directory)) != NULL) {
				if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) continue;
				
				std::string dirpath = path + "/";
				dirpath += file->d_name;
				
				struct stat file_stat;
				if(stat(dirpath.c_str(), &file_stat) == 0) {
					if(S_ISDIR(file_stat.st_mode)) {
						/*dirpath += "/monitor.conf";*/
						if(stat((dirpath + "/monitor.conf").c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
							processFile(dirpath + "/monitor.conf");
							traverse(std::string(file->d_name) + ".module-path")->setData(dirpath);
						}
					}
				}
			}
		}
		
		closedir(directory);
	} while(searchPaths.length());
}

void Configuration::processArgv() {
	int i = 0;
	
	/* This will occur in a very few circumstances, mainly mangled exec() calls. */
	if(m_argv[0] == NULL) return;
	
	bool eoaFound = false;
	
	while(m_argv[++i] != NULL) {
		
		if(eoaFound) {
			addLaunchArgument(m_argv[i]);
			continue;
		}
		else if(m_argv[i][0] != '-' || m_argv[i][1] != '-') {
			/*LogSystem::logConfigurationMessage(Misc::StreamAsString()
				<< "Mangled argument \"" << m_argv[i] << "\"; all arguments begin with a double dash.");*/
			eoaFound = true;
			addLaunchArgument(m_argv[i]);
			continue;
		}
		std::string argument = &m_argv[i][2];
		if(argument.length() == 0) {
			eoaFound = true;
			continue;
		}
		
		std::string::size_type divider = argument.find('=');
		if(divider == std::string::npos) {
			traverse(argument)->setData("true");
		}
		else {
			traverse(argument.substr(0, divider))->setData(argument.substr(divider+1));
		}
	}
}

ConfigurationItem *Configuration::traverse(std::string path) {
	std::string::size_type divider = path.find('.');
	std::string moduleName;
	if(divider != std::string::npos) {
		/*LogSystem::logConfigurationMessage(Misc::StreamAsString() << "Poorly-formed traverse path \"" << path << "\"; no item specified.");*/
		/*return NULL;*/
		moduleName = path.substr(0, divider);
		path.erase(0, divider+1);
	}
	else moduleName = "global";
	
	ConfigurationModule *module = this->module(moduleName);
	
	return module->item(path);
}

void Configuration::addLaunchArgument(const std::string &argument) {
	m_launchArguments.push_back(argument);
}

} // namespace Misc
