#ifndef AesalonArtisan_cpuTime_Viewport_H
#define AesalonArtisan_cpuTime_Viewport_H

#include "artisan/gviewport/Viewport.h"
#include "DataStore.h"

class Viewport : public Artisan::GViewport::Viewport {
private:
	DataStore *m_dataStore;
public:
	Viewport(DataStore *dataStore);
	virtual ~Viewport();
};

#endif
