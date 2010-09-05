#ifndef SessionVisualization_H
#define SessionVisualization_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include "visualization/VisualizationWidget.h"

class SessionVisualization : public QFrame { Q_OBJECT
public:
	SessionVisualization();
	virtual ~SessionVisualization();
private:
	QVBoxLayout *m_layout;
	QLabel *m_titleLabel;
public:
	QLabel *titleLabel() const { return m_titleLabel; }
	
	void setVisualization(VisualizationWidget *widget);;
private slots:
	void handleMenuRequest(QPoint pos);
signals:
	void contextMenuRequest(QPoint globalPos, SessionVisualization *visualization);
};

#endif
