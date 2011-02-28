#ifndef AesalonArtisan_memory_Viewport_H
#define AesalonArtisan_memory_Viewport_H

#include "artisan/Viewport.h"
#include "artisan/gviewport/BasicViewport.h"

class Viewport : public Artisan::GViewport::BasicViewport {
public:
	Viewport(Artisan::GViewport::Data *data);
	virtual ~Viewport();
};


#endif
