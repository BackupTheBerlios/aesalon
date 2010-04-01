#include "BlockCountFactory.h"
#include "BlockCountFormatter.h"
#include "BlockCountRenderer.h"

BlockCountFactory::BlockCountFactory(DataThread *data_thread) : VisualizationFactory(data_thread) {

}


AxisFormatter *BlockCountFactory::create_formatter() const {
    return new BlockCountFormatter();
}

Renderer *BlockCountFactory::create_renderer() const {
    return new BlockCountRenderer();
}
