/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/DataOutputController.cpp
*/

#include "monitor/DataOutputController.h"
#include "config/GlobalVault.h"
#include "monitor/LogOutput.h"

#include "util/MessageSystem.h"

namespace Monitor {

DataOutputController::DataOutputController() {
	std::vector<std::string> outputVector;
	
	Config::GlobalVault::instance()->get("monitor:output", outputVector);
	
	for(int i = 0; i < int(outputVector.size()); i ++) {
		DataOutput *output = createOutput(outputVector[i]);
		if(output != NULL) m_dataOutputVector.push_back(output);
	}
	
	if(outputVector.size() == 0) Message(Warning, "No data outputs specified!");
}

DataOutputController::~DataOutputController() {
	for(int i = 0; i < int(m_dataOutputVector.size()); i ++) {
		delete m_dataOutputVector[i];
	}
}

void DataOutputController::output(Comm::Packet *packet) {
	for(int i = 0; i < int(m_dataOutputVector.size()); i ++) {
		m_dataOutputVector[i]->output(packet);
	}
}

DataOutput *DataOutputController::createOutput(const std::string &spec) {
	std::string type, content;
	std::string::size_type index = spec.find(":");
	type = spec.substr(0, index);
	content = spec.substr(index+1);
	
	if(type == "tcp") {
		Message(Fatal, "TCP data output NYI.");
	}
	else if(type == "log") {
		return new LogOutput(content);
	}
	else Message(Warning, "Unknown output type \"" << type << "\"");
	return NULL;
}

} // namespace Monitor
