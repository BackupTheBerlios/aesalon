#include "Interface.h"
#include "Viewport.h"

InstantiateArtisan(Interface)

Interface::Interface() {
	m_storage = new DataStore();
}

Interface::~Interface() {

}

Artisan::DataStore *Interface::dataStore() {
	return m_storage;
}

Artisan::Viewport *Interface::createViewport() {
	return new Viewport(m_storage);
}
