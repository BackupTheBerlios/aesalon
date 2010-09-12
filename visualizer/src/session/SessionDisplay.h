#ifndef SessionDisplay_H
#define SessionDisplay_H

#include <QWidget>
#include <QGridLayout>
#include <QMenu>
#include <QTimer>
#include <QMenuBar>

#include "module/ModuleMapper.h"
#include "SessionVisualization.h"
#include "SessionHeartbeat.h"

class Session;

class SessionDisplay : public QWidget { Q_OBJECT
public:
	SessionDisplay(Session *session);
	virtual ~SessionDisplay();
private:
	ModuleMapper *m_moduleMapper;
	QGridLayout *m_grid;
	QMenu *m_contextMenu;
	SessionVisualization *m_contextVisualization;
	SessionHeartbeat *m_heartbeat;
	Session *m_session;
	QMenuBar *m_menuBar;
public:
	ModuleMapper *moduleMapper() const { return m_moduleMapper; }
	QGridLayout *grid() const { return m_grid; }
	
	virtual void resizeEvent(QResizeEvent *event);
private:
	SessionVisualization *newVisualization() const;
	void setupMenuBar();
public slots:
	void addColumn();
	void addRow();
	
	void setWidget(QPoint which, QWidget *widget);
private slots:
	void displayContextMenu(QPoint globalPosition, SessionVisualization *visualization);
	void setVisualizationModule(SessionVisualization *visualization, int moduleID);
	void setVisualizationModule(QAction *action);
	
	void showSettings();
signals:
	void updateVisualizations();
};

#endif
