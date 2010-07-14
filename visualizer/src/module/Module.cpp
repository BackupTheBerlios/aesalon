#include <sys/stat.h>
#include <dlfcn.h>

#include <QSettings>

#include "Module.h"

Module::Module(const char *name) : m_name(name), m_moduleHandle(NULL){
	QString path = modulePath(QString("lib").append(name).append("Visualizer.so"));
	if(path.length() == 0) return;
	m_moduleHandle = dlopen(path.toAscii().constData(), RTLD_NOW | RTLD_LOCAL);
	if(m_moduleHandle == NULL) {
		qWarning("Cannot load module \"%s\".", name);
		return;
	}
	
	void *instantiationHandle = dlsym(m_moduleHandle, "AesalonVisualizerCreateInstance");
	
	if(instantiationHandle == NULL) {
		qWarning("Module \"%s\" does not have instantiation function.", name);
	}
	
	ModuleInterface *(*instantiateFunction)();
	*(void **)(&instantiateFunction) = instantiationHandle;
	
	m_interface = instantiateFunction();
	
	if(m_interface == NULL) {
		qWarning("Module \"%s\" failed to create ModuleInterface instance.", name);
		return;
	}
}

Module::~Module() {
	
}

void Module::processIncoming(DataPacket *packet) {
	if(m_interface == NULL) return;
	m_interface->processIncoming(packet);
}

QString Module::modulePath(QString filename) {
	QSettings settings;
	QString pathList = settings.value("module-path", "modules/build/").toString();
	if(filename.length() == 0 || pathList.length() == 0) return "";
	
	do {
		QString path = pathList.left(pathList.indexOf(":"));
		
		QString filePath = path;
		filePath += "/";
		filePath += filename;
		
		struct stat possibleStat;
		if(stat(filePath.toAscii().constData(), &possibleStat) == 0) return filePath;
		
		pathList.remove(0, path.length()+1);
	} while(pathList.contains(":"));
	
	return "";
}
