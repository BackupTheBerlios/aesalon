#include "Viewport.h"
#include "util/MessageSystem.h"

Viewport::Viewport(DataStore *dataStore) : Artisan::GViewport::BasicViewport(dataStore), m_dataStore(dataStore) {
	Message(Debug, "Creating cpuTime Viewport instance . . .");
	updateRange(Artisan::GViewport::Rect(1.0, 1.0));
}

Viewport::~Viewport() {
	
}
