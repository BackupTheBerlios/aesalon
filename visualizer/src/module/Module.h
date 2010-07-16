#ifndef Module_H
#define Module_H

#include <QString>

#include "interface/Interface.h"
#include "storage/DataRange.h"

class Module {
public:
	Module(const char *name);
	~Module();
private:
	QString m_name;
	void *m_moduleHandle;
	VisualizerInterface *m_interface;
public:
	const QString &name() const { return m_name; }
	VisualizerInterface *interface() const { return m_interface; }
	
	void processIncoming(DataPacket *packet);
	void visualize(Visualization *visualization);
	DataRange defaultDataRange();
private:
	QString modulePath(QString filename);
};

#endif
