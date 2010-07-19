#ifndef VisualizationWidget_H
#define VisualizationWidget_H

#include <QWidget>

#include "Visualization.h"
#include "module/Module.h"
#include "VisualizationController.h"

class VisualizationWidget : public QWidget { Q_OBJECT
public:
	VisualizationWidget(Module *module);
	virtual ~VisualizationWidget();
private:
	Module *m_module;
	Visualization *m_visualization;
	VisualizationController *m_controller;
	QPoint m_lastMousePos;
public:
	Visualization *visualization() const { return m_visualization; }
	
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif
