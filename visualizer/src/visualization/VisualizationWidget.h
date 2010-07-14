#ifndef VisualizationWidget_H
#define VisualizationWidget_H

#include <QWidget>

#include "Visualization.h"

class VisualizationWidget : public QWidget { Q_OBJECT
public:
	VisualizationWidget(DataRange range);
	virtual ~VisualizationWidget();
private:
	Visualization *m_visualization;
public:
	Visualization *visualization() const { return m_visualization; }
	
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
};

#endif
