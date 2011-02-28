#include "Interface.h"

#include "Viewport.h"

InstantiateArtisan(Interface)

Interface::Interface() {
	m_storage = new DataStore();
}

Interface::~Interface() {

}

Artisan::Viewport *Interface::createViewport() {
	return new Viewport(m_storage->data());
}
