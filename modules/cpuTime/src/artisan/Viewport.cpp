#include <QListIterator>

#include "Viewport.h"
#include "util/MessageSystem.h"

#include "artisan/gviewport/LineObject.h"

Viewport::Viewport(DataStore *dataStore) : Artisan::GViewport::BasicViewport(dataStore), m_dataStore(dataStore) {
	Message(Debug, "Creating cpuTime Viewport instance . . .");
	updateRange(Artisan::GViewport::Rect(1.0, 1.0));
	
	Artisan::GViewport::Object *object = NULL;
	
	double lastTime = 0.0;
	double lastData = 0.0;
	
	QListIterator<QPair<double, double> > i = m_dataStore->iterator();
	while(i.hasNext()) {
		QPair<double, double> pair = i.next();
		
		if(lastTime != 0.0 || lastData != 0.0) {
			object = new Artisan::GViewport::LineObject(
				Artisan::GViewport::Point(lastTime, lastData),
				Artisan::GViewport::Point(pair.first, pair.second));
			
			data()->addObject(object);
		}
		lastTime = pair.first;
		lastData = pair.second;
	}
	
	Artisan::GViewport::TreeType::Bound maxBound = data()->tree().bounds();
	
	updateRange(Artisan::GViewport::Rect(maxBound.range(0).start(), maxBound.range(0).end(), maxBound.range(1).start(),
		maxBound.range(1).end()));
}

Viewport::~Viewport() {
	
}
