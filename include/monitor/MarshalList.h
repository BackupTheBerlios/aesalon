/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/MarshalList.h
*/

#ifndef AesalonMonitor_MarshalList_H
#define AesalonMonitor_MarshalList_H

#include <string>
#include <vector>
#include <map>

#include "monitor/MarshalWrapper.h"

namespace Monitor {

class MarshalList {
protected:
	typedef std::vector<MarshalWrapper *> MarshalVector;
private:
	MarshalVector m_marshalVector;
public:
	MarshalList();
	~MarshalList();
	
	MarshalWrapper *marshal(ModuleID moduleID);
	void loadMarshal(const std::string &name);
};

} // namespace Monitor

#endif
