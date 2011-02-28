#ifndef AesalonArtisan_cpuTime_DataStore_H
#define AesalonArtisan_cpuTime_DataStore_H

#include <QPair>
#include <QList>
#include <QListIterator>

#include "artisan/DataStore.h"
#include "artisan/gviewport/Data.h"

class DataStore : public Artisan::DataStore, public Artisan::GViewport::Data {
private:
	QList<QPair<double, double> > m_dataList;
	double m_firstTime;
	double m_lastCpuTime;
	double m_lastTime;
public:
	DataStore();
	virtual ~DataStore();
	
	virtual void process(Comm::Packet *packet);
	
	QListIterator<QPair<double, double> > iterator() { return QListIterator<QPair<double, double> >(m_dataList); }
};


#endif
