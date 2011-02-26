#ifndef AesalonArtisan_cpuTime_DataStore_H
#define AesalonArtisan_cpuTime_DataStore_H

#include "artisan/DataStore.h"

class DataStore : public Artisan::DataStore {
public:
	DataStore();
	virtual ~DataStore();
	
	virtual void process(Comm::Packet *packet);
};

#endif
