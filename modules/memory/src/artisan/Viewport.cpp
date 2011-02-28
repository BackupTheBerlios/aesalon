#include "Viewport.h"

#include "util/MessageSystem.h"

Viewport::Viewport(Artisan::GViewport::Data *data) : Artisan::GViewport::BasicViewport(data) {
	Artisan::GViewport::TreeType::Bound bound = data->tree().bounds();
	setViewport(Artisan::GViewport::Rect(
		bound.range(0).start(), bound.range(0).end(),
		bound.range(1).start(), bound.range(1).end()));
	
	Message(Debug, "Data range: " << bound.range(0).start() << " to " << bound.range(0).end() << ", "
		<< bound.range(1).start() << " to " << bound.range(1).end());
}

Viewport::~Viewport() {

}
