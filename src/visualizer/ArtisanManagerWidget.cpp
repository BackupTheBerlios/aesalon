/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/ArtisanManagerWidget.cpp
*/

#include <QList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidgetItem>
#include <QString>

#include "visualizer/ArtisanManagerWidget.h"

namespace Visualizer {

ArtisanManagerWidget::ArtisanManagerWidget(ArtisanManager *artisanManager) : m_artisanManager(artisanManager) {
	QVBoxLayout *layout = new QVBoxLayout();
	
	m_listWidget = new QListWidget();
	layout->addWidget(m_listWidget);
	
	QPushButton *createButton = new QPushButton(tr("Add artisan &viewport"));
	layout->addWidget(createButton);
	connect(createButton, SIGNAL(clicked()), this, SLOT(newArtisan()));
	
	setLayout(layout);
	
	connect(m_artisanManager, SIGNAL(newArtisan(std::string)), this, SLOT(artisanLoaded(std::string)));
	foreach(std::string name, m_artisanManager->artisanList()) {
		artisanLoaded(name);
	}
}

ArtisanManagerWidget::~ArtisanManagerWidget() {
	
}

void ArtisanManagerWidget::newArtisan() {
	QList<QListWidgetItem *> itemList = m_listWidget->selectedItems();
	
	foreach(QListWidgetItem *item, itemList) {
		const std::string &name = item->data(Qt::DisplayRole).toString().toStdString();
		Artisan::Viewport *viewport = m_artisanManager->artisan(name)->interface()->createViewport();
		emit newViewport(viewport);
	}
}

void ArtisanManagerWidget::artisanLoaded(std::string name) {
	ArtisanWrapper *artisan = m_artisanManager->artisan(name);
	if(artisan != NULL) new QListWidgetItem(QString::fromStdString(name), m_listWidget);
}

} // namespace Visualizer
