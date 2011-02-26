#ifndef AesalonArtisan_cpuTime_Storage_H
#define AesalonArtisan_cpuTime_Storage_H

#include "artisan/Storage.h"

class Storage : public Artisan::Storage {
public:
	Storage();
	virtual ~Storage();
	
	virtual void process(Comm::Packet *packet);
};

#endif
