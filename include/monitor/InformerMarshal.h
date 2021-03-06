/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/InformerMarshal.h
*/

#ifndef AesalonMonitor_InformerMarshal_H
#define AesalonMonitor_InformerMarshal_H

#include "marshal/Interface.h"

namespace Monitor {

class InformerMarshal : public Marshal::Interface {
public:
	InformerMarshal();
	virtual ~InformerMarshal();
	
	virtual Comm::Packet *marshal(Comm::Packet *packet);
private:
	void moduleLoaded(Comm::Packet *packet);
	void fileLoaded(Comm::Packet *packet);
	void processExited(Comm::Packet *packet);
};

} // namespace Monitor

#endif
