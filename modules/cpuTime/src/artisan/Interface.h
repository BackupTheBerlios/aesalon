#ifndef AesalonArtisan_cpuTime_Interface_H
#define AesalonArtisan_cpuTime_Interface_H

#include "artisan/Interface.h"
#include "Storage.h"

class Interface : public Artisan::Interface {
private:
	Storage *m_storage;
public:
	Interface();
	virtual ~Interface();
	
	virtual Artisan::Storage *storageObject();
	virtual Artisan::Viewport *createViewport();
};

#endif
