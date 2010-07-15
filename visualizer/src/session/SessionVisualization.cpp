#include "SessionVisualization.h"
#include "SessionVisualization.moc"

SessionVisualization::SessionVisualization() {
	m_layout = new QVBoxLayout();
	
	setFrameShape(Panel);
	setFrameShadow(Sunken);
	
	m_titleLabel = new QLabel(tr("Empty"));
	m_titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	m_titleLabel->setContextMenuPolicy(Qt::CustomContextMenu);
	m_layout->addWidget(m_titleLabel);
	
	connect(m_titleLabel, SIGNAL(customContextMenuRequested(QPoint)), SLOT(handleMenuRequest(QPoint)));
	
	setLayout(m_layout);
}

SessionVisualization::~SessionVisualization() {
	
}

void SessionVisualization::setVisualization(VisualizationWidget *widget) {
	m_layout->insertWidget(1, widget, 1);
}

void SessionVisualization::handleMenuRequest(QPoint pos) {
	emit contextMenuRequest(m_titleLabel->mapToGlobal(pos), this);
}
