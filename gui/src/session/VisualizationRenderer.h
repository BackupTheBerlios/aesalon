#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H

#include <QImage>

class VisualizationRenderer {
private:
    QImage *image;
public:
    VisualizationRenderer(QImage *image);
    virtual ~VisualizationRenderer();
    
    void update();
};

#endif
