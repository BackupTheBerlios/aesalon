#ifndef Module_H
#define Module_H

#include <QString>

#include "renderer/Controller.h"
#include "renderer/DataRange.h"

class Visualization;

class Module {
public:
	Module(const char *name);
	~Module();
private:
	QString m_name;
	void *m_moduleHandle;
	RendererController *m_controller;
public:
	const QString &name() const { return m_name; }
	RendererController *controller() const { return m_controller; }
	
	void processIncoming(DataPacket *packet);
	Renderer *renderer(std::string name);
	DataRange defaultDataRange();
private:
	QString modulePath(QString filename);
};

#endif
