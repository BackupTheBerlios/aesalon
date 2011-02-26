#include "Interface.h"
#include "Viewport.h"

InstantiateArtisan(Interface)

Interface::Interface() {
	m_storage = new Storage();
}

Interface::~Interface() {

}

Artisan::Storage *Interface::storageObject() {
	return m_storage;
}

Artisan::Viewport *Interface::createViewport() {
	return new Viewport();
}
