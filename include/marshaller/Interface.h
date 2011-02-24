/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/marshaller/Interface.h
*/

#ifndef AesalonMarshaller_Interface_H
#define AesalonMarshaller_Interface_H

#include "comm/Packet.h"

namespace Marshaller {

class Interface {
public:
	virtual ~Interface() {}
	
	virtual Comm::Packet *marshal(Comm::Packet *packet) = 0;
};

} // namespace Marshaller

#endif
