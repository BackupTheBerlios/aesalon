#include "Viewport.h"
#include "util/MessageSystem.h"

#include "artisan/gviewport/LineObject.h"

Viewport::Viewport(DataStore *dataStore) : Artisan::GViewport::BasicViewport(dataStore), m_dataStore(dataStore) {
	Message(Debug, "Creating cpuTime Viewport instance . . .");
	updateRange(Artisan::GViewport::Rect(1.0, 1.0));
	
	Artisan::GViewport::Object *object =
		new Artisan::GViewport::LineObject(
			Artisan::GViewport::Point(0.0, 0.0),
			Artisan::GViewport::Point(0.5, 0.7));
	
	data()->addObject(object);
}

Viewport::~Viewport() {
	
}
