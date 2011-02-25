/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/MarshalList.cpp
*/

#include "monitor/MarshalList.h"
#include "util/MessageSystem.h"
#include "monitor/InformerMarshal.h"
#include "monitor/Coordinator.h"
#include "util/StringTo.h"

namespace Monitor {

MarshalList::MarshalList() {
	MarshalWrapper *marshal = new MarshalWrapper(new InformerMarshal());
	m_marshalVector.push_back(marshal);
}

MarshalList::~MarshalList() {
	
}

MarshalWrapper *MarshalList::marshal(ModuleID moduleID) {
	if(moduleID >= m_marshalVector.size()) {
		return NULL;
	}
	return m_marshalVector[moduleID];
}

void MarshalList::loadMarshal(const std::string &name) {
	MarshalWrapper *marshal = new MarshalWrapper(name);
	if(marshal->interface() == NULL) {
		Message(Warning, "Could not load marshal for module " << name << ".");
		delete marshal;
	}
	else {
		ModuleID moduleID = Util::StringTo<ModuleID>(Coordinator::instance()->vault()->get(name + ":moduleID"));
		
		if(moduleID >= m_marshalVector.size()) m_marshalVector.resize(moduleID+1);
		m_marshalVector[moduleID] = marshal;
	}
}

} // namespace Monitor
