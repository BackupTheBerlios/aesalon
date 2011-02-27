#ifndef AesalonArtisan_cpuTime_Viewport_H
#define AesalonArtisan_cpuTime_Viewport_H

#include "artisan/Viewport.h"
#include "artisan/gviewport/BasicViewport.h"
#include "DataStore.h"

class Viewport : public Artisan::GViewport::BasicViewport {
private:
	DataStore *m_dataStore;
	double m_prevX, m_prevY;
public:
	Viewport(DataStore *dataStore);
	virtual ~Viewport();
};

#endif
