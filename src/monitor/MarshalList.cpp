/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/MarshalList.cpp
*/

#include "monitor/MarshalList.h"
#include "util/MessageSystem.h"
#include "monitor/InformerMarshal.h"
#include "config/GlobalVault.h"
#include "util/StringTo.h"

namespace Monitor {

MarshalList::MarshalList() {
	MarshalWrapper *marshal = new MarshalWrapper(new InformerMarshal());
	m_marshalVector.push_back(marshal);
}

MarshalList::~MarshalList() {
	for(MarshalVector::iterator i = m_marshalVector.begin(); i != m_marshalVector.end(); ++i) {
		delete *i;
	}
}

MarshalWrapper *MarshalList::marshal(ModuleID moduleID) {
	if(moduleID >= m_marshalVector.size()) {
		return NULL;
	}
	return m_marshalVector[moduleID];
}

void MarshalList::loadMarshal(const std::string &name) {
	/* Check to see if the marshal has already been loaded. */
	ModuleID moduleID = Util::StringTo<ModuleID>(Config::GlobalVault::instance()->get(name + ":moduleID"));
	if(moduleID < m_marshalVector.size() && m_marshalVector[moduleID] != NULL) return;
	
	MarshalWrapper *marshal = new MarshalWrapper(name);
	if(marshal->interface() == NULL) {
		Message(Warning, "Could not load marshal for module " << name << ".");
		delete marshal;
	}
	else {
		if(moduleID >= m_marshalVector.size()) m_marshalVector.resize(moduleID+1);
		m_marshalVector[moduleID] = marshal;
	}
}

} // namespace Monitor
