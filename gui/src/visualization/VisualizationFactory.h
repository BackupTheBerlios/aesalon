#ifndef AESALON_GUI_VISUALIZATION_FACTORY_H
#define AESALON_GUI_VISUALIZATION_FACTORY_H

#include <QWidget>
#include <QImage>

class VisualizationDisplay;
class VisualizationRenderer;

class VisualizationFactory {
public:
    VisualizationFactory();
    virtual ~VisualizationFactory();
    
    virtual VisualizationDisplay *create_display(QWidget *parent) const = 0;
    virtual VisualizationRenderer *create_renderer(QImage *image) const = 0;
};

#endif
