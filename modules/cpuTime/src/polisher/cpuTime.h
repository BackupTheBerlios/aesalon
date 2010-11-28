/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file modules/cpuTime/src/polisher/cpuTime.h

*/

#ifndef AesalonModule_cpuTime_PolisherInterface_H
#define AesalonModule_cpuTime_PolisherInterface_H


#include "common/PolisherInterface.h"

class CpuTime : public Common::PolisherInterface {
public:
	virtual ~CpuTime();
	
	virtual Packet *polish(Packet *packet);
};

extern "C" {

Common::PolisherInterface *AP_Instantiate();

} // extern "C"

#endif
