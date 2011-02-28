#ifndef AesalonArtisan_memory_Interface_H
#define AesalonArtisan_memory_Interface_H

#include "artisan/Interface.h"
#include "DataStore.h"

class Interface : public Artisan::Interface {
private:
	DataStore *m_storage;
public:
	Interface();
	virtual ~Interface();
	
	virtual Artisan::DataStore *dataStore() { return m_storage; }
	virtual Artisan::Viewport *createViewport();
};

#endif
