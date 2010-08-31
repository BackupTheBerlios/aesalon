#include <sys/stat.h>
#include <dlfcn.h>

#include <QSettings>

#include "Module.h"
#include "visualization/Visualization.h"

Module::Module(const char *name) : m_name(name), m_moduleHandle(NULL) {
	qDebug("Attempting to load module \"%s\" . . .", name);
	QString path = modulePath(QString(name).append("/visualizer.so"));
	qDebug("to parse: %s", qPrintable(QString(name).append("/visualizer.so")));
	qDebug("path: %s", qPrintable(path));
	if(path.length() == 0) {
		qWarning("Cannot load module; does not exist in module path.");
		return;
	}
	m_moduleHandle = dlopen(path.toAscii().constData(), RTLD_LAZY | RTLD_LOCAL);
	if(m_moduleHandle == NULL) {
		qWarning("Cannot load module \"%s\". Error: %s", name, dlerror());
		qWarning("Last path is %s.", path.toAscii().constData());
		return;
	}
	
	void *instantiationHandle = dlsym(m_moduleHandle, "AesalonVisualizerCreateInstance");
	
	if(instantiationHandle == NULL) {
		qWarning("Module \"%s\" does not have instantiation function.", name);
		return;
	}
	
	VisualizerModule::Controller *(*instantiateFunction)();
	*(void **)(&instantiateFunction) = instantiationHandle;
	
	m_controller = instantiateFunction();
	
	if(m_controller == NULL) {
		qWarning("Module \"%s\" failed to create ModuleInterface instance.", name);
		return;
	}
	qDebug("Loaded module \"%s\" properly.", name);
}

Module::~Module() {
	
}

void Module::processIncoming(DataPacket *packet) {
	if(m_controller == NULL) return;
	m_controller->dataCache()->processPacket(packet);
}

VisualizerModule::Renderer *Module::createRenderer(std::string name) {
	if(m_controller == NULL) return NULL;
	return m_controller->factory()->createRenderer(name);
}

DataRange Module::defaultDataRange() {
	/*if(m_interface == NULL) return DataRange();
	return m_interface->defaultDataRange();*/
	/* TODO: implement this. */
	return DataRange();
}

QString Module::modulePath(QString filename) {
	QSettings settings;
	QString pathList = settings.value("module-path", "module/modules/").toString();
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
