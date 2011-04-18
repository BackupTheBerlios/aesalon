#include <QListIterator>

#include "Viewport.h"
#include "util/MessageSystem.h"

#include "artisan/gviewport/LineObject.h"

Viewport::Viewport(DataStore *dataStore) : Artisan::GViewport::BasicViewport(dataStore), m_dataStore(dataStore) {
	Message(Debug, "Creating cpuTime Viewport instance . . .");
	setViewport(Artisan::GViewport::Rect(1.0, 1.0));
	
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
	
	Artisan::GViewport::TreeType::BoundType maxBound = data()->tree().bounds();
	
	setViewport(Artisan::GViewport::Rect(maxBound.start(0), maxBound.end(0), maxBound.start(1), maxBound.end(1)));
}

Viewport::~Viewport() {
	
}
