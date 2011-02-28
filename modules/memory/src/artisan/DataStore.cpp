#include "DataStore.h"

#include "util/MessageSystem.h"
#include "artisan/gviewport/RectObject.h"

DataStore::DataStore() {
	m_data = new Artisan::GViewport::Data();
}

DataStore::~DataStore() {
	
}

void DataStore::process(Comm::Packet *packet) {
	uint8_t type = packet->data()[0];
	
	if(type == 0) Message(Warning, "Unknown memory packet type . . .");
	else if(type == 1) {
		Message(Debug, "calloc() packet . . .");
	}
	else if(type == 2) {
		uint64_t *data = reinterpret_cast<uint64_t *>(packet->data() + 1);
		Message(Debug, "malloc() packet . . .");
		uint64_t timestamp = data[0];
		uint64_t address = data[1];
		uint64_t size = data[2];
		Artisan::GViewport::Object *object = new Artisan::GViewport::RectObject(
			Artisan::GViewport::Rect(timestamp, -1, address, address+size));
		m_data->addObject(object);
	}
	else if(type == 3) {
		Message(Debug, "free() packet . . .");
	}
	else if(type == 4) {
		Message(Debug, "realloc() packet . . .");
	}
}
