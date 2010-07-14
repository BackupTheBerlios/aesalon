#ifndef Module_H
#define Module_H

#include <QString>

#include "interface/Interface.h"

class Module {
public:
	Module(const char *name);
	~Module();
private:
	QString m_name;
	void *m_moduleHandle;
	ModuleInterface *m_interface;
public:
	const QString &name() const { return m_name; }
	ModuleInterface *interface() const { return m_interface; }
	
	void processIncoming(DataPacket *packet);
private:
	QString modulePath(QString filename);
};

#endif
