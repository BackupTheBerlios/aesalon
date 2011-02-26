/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/ArtisanManagerWidget.h
*/

#ifndef AesalonVisualizer_ArtisanManagerWidget_H
#define AesalonVisualizer_ArtisanManagerWidget_H

#include <QWidget>
#include <QListWidget>

#include "ArtisanManager.h"

namespace Visualizer {

class ArtisanManagerWidget : public QWidget { Q_OBJECT
private:
	ArtisanManager *m_artisanManager;
	QListWidget *m_listWidget;
public:
	ArtisanManagerWidget(ArtisanManager *artisanManager);
	virtual ~ArtisanManagerWidget();
private slots:
	void newArtisan();
	void artisanLoaded(std::string name);
signals:
	void newViewport(Artisan::Viewport *viewport);
};

} // namespace Visualizer

#endif
