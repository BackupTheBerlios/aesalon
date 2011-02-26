#include "Viewport.h"
#include "artisan/gviewport/RectObject.h"
#include "util/MessageSystem.h"

Viewport::Viewport(DataStore *dataStore) : m_dataStore(dataStore) {
	Message(Debug, "Creating cpuTime Viewport instance . . .");
	/*Artisan::GViewport::RectObject *ro = new Artisan::GViewport::RectObject(0, 0, 0, 100, 100);
	addObject(ro);*/
}

Viewport::~Viewport() {
	
}
