#include "Viewport.h"
#include "util/MessageSystem.h"

Viewport::Viewport(DataStore *dataStore) : Artisan::GViewport::BasicViewport(dataStore), m_dataStore(dataStore) {
	Message(Debug, "Creating cpuTime Viewport instance . . .");
}

Viewport::~Viewport() {
	
}
