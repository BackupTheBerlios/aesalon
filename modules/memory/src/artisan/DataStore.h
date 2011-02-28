#ifndef AesalonArtisan_memory_DataStore_H
#define AesalonArtisan_memory_DataStore_H

#include <QPair>
#include <QList>
#include <QListIterator>

#include "artisan/DataStore.h"
#include "artisan/gviewport/Data.h"

class DataStore : public Artisan::DataStore {
private:
	Artisan::GViewport::Data *m_data;
public:
	DataStore();
	virtual ~DataStore();
	
	Artisan::GViewport::Data *data() const { return m_data; }
	
	virtual void process(Comm::Packet *packet);
};

#endif
