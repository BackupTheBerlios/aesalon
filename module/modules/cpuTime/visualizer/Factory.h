#ifndef CpuTimeFactory_H
#define CpuTimeFactory_H

#include <map>

#include "visualizer/AbstractFactory.h"

#include "DataCache.h"

namespace VisualizerModule {

class CpuTimeFactory : public AbstractFactory<CpuTimeDataCache> {
public:
	CpuTimeFactory();
	virtual ~CpuTimeFactory();
private:
	typedef std::map<std::string, Renderer *> RendererMap;
	RendererMap m_rendererMap;
public:
	virtual Renderer *createRenderer(const std::string &name) const;
	virtual void rendererNames(std::vector<std::string> &names) const;
};

} // namespace VisualizerModule

#endif
