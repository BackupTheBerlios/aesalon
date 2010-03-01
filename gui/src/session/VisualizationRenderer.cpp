#include "VisualizationRenderer.h"

VisualizationRenderer::VisualizationRenderer(QImage *image, bool can_split)
    : image(image), can_split(can_split) {
    
}

VisualizationRenderer::~VisualizationRenderer() {
    
}


void VisualizationRenderer::recalc_ranges() {
    ranges.clear();
    if(can_split == false) {
        VisualizationRenderDataRange range = VisualizationRenderDataRange();
        foreach(VisualizationRenderObject *object, render_object_list) {
            if(object->get_data_range().get_lower_time() < range.get_lower_time())
                range.set_lower_time(object->get_data_range().get_lower_time());
            if(object->get_data_range().get_upper_time() < range.get_upper_time())
                range.set_upper_time(object->get_data_range().get_upper_time());
            
            if(object->get_data_range().get_lower_data() < range.get_lower_data())
                range.set_lower_data(object->get_data_range().get_lower_data());
            if(object->get_data_range().get_upper_data() < range.get_upper_data())
                range.set_upper_data(object->get_data_range().get_upper_data());
        }
        ranges.append(range);
    }
    else {
        qCritical("Compressable range calculation NYI!");
        return;
    }
}

void VisualizationRenderer::paint_grid() {
    /* Just ignore the grid for now . . . */
}

void VisualizationRenderer::render_objects() {
    foreach(VisualizationRenderObject *object, render_object_list) {
        object->paint_onto(image);
    }
}

void VisualizationRenderer::update() {
    recalc_ranges();
    /* Paint the image white . . . */
    image->fill(qRgb(255, 255, 255));
    paint_grid();
    render_objects();
}

void VisualizationRenderer::add_data(VisualizationData *data) {
    data->create_r_objects(this);
}

void VisualizationRenderer::add_render_object(VisualizationRenderObject *r_object) {
    render_object_list.append(r_object);
}
