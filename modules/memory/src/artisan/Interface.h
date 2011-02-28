#ifndef AesalonArtisan_cpuTime_Interface_H
#define AesalonArtisan_cpuTime_Interface_H

#include "artisan/Interface.h"
#include "DataStore.h"

class Interface : public Artisan::Interface {
private:
	DataStore *m_storage;
public:
	Interface();
	virtual ~Interface();
	
	virtual Artisan::DataStore *dataStore();
	virtual Artisan::Viewport *createViewport();
};

#endif
