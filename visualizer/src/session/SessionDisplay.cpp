#include <QResizeEvent>
#include <QLabel>
#include <QMenu>
#include <QMenuItem>

#include "SessionDisplay.h"
#include "SessionDisplay.moc"
#include "SessionVisualization.h"

SessionDisplay::SessionDisplay(ModuleMapper *moduleMapper) : QWidget(NULL), m_moduleMapper(moduleMapper) {
	m_grid = new QGridLayout();
	SessionVisualization *sv = new SessionVisualization();
	connect(sv, SIGNAL(contextMenuRequest(QPoint,SessionVisualization*)), SLOT(displayContextMenu(QPoint,SessionVisualization*)));
	m_grid->addWidget(sv, 0, 0);
	
	setLayout(m_grid);
}

SessionDisplay::~SessionDisplay() {
	
}

void SessionDisplay::resizeEvent(QResizeEvent *event) {
	
}

void SessionDisplay::addColumn() {
	m_grid->addWidget(new SessionVisualization(), 0, m_grid->columnCount());
	for(int i = 1; i < m_grid->rowCount(); i ++) {
		m_grid->addWidget(new SessionVisualization(), i, m_grid->columnCount()-1);
	}
}

void SessionDisplay::addRow() {
	m_grid->addWidget(new SessionVisualization(), m_grid->rowCount(), 0);
	for(int i = 1; i < m_grid->columnCount(); i ++) {
		m_grid->addWidget(new SessionVisualization(), m_grid->rowCount()-1, i);
	}
}

void SessionDisplay::setWidget(QPoint which, QWidget *widget) {
	m_grid->addWidget(widget, which.x(), which.y());
}

void SessionDisplay::displayContextMenu(QPoint globalPosition, SessionVisualization *visualization) {
	m_contextMenu = new QMenu(this);
	
	QMenu *moduleMenu = m_contextMenu->addMenu(tr("Modules"));
	
	for(int i = 1; i < m_moduleMapper->moduleCount(); i ++) {
		moduleMenu->addAction(m_moduleMapper->module(i)->name(), this, SIGNAL(setVisualizationModule(visualization, i)));
	}
	
	m_contextMenu->popup(globalPosition);
}

void SessionDisplay::setVisualizationModule(SessionVisualization *visualization, int moduleID) {
	Module *module = m_moduleMapper->module(moduleID);
	
	visualization->setVisualization(new VisualizationWidget(module));
	
}
