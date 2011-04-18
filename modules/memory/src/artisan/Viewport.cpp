#include "Viewport.h"

#include "util/MessageSystem.h"

Viewport::Viewport(Artisan::GViewport::Data *data) : Artisan::GViewport::BasicViewport(data) {
	Artisan::GViewport::TreeType::BoundType bound = data->tree().bounds();
	setViewport(Artisan::GViewport::Rect(
		bound.start(0), bound.end(0),
		bound.start(1), bound.end(1)));
	
	/*Message(Debug, "Data range: " << bound.range(0).start() << " to " << bound.range(0).end() << ", "
		<< bound.range(1).start() << " to " << bound.range(1).end());*/
}

Viewport::~Viewport() {

}
