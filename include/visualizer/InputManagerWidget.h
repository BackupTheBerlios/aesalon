/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/InputManagerWidget.h
*/

#ifndef AesalonVisualizer_InputManagerWidget_H
#define AesalonVisualizer_InputManagerWidget_H

#include <QDockWidget>
#include <QListView>

#include "InputManager.h"
#include "InputCreator.h"

namespace Visualizer {

class InputManagerWidget : public QDockWidget { Q_OBJECT
private:
	InputManager *m_inputManager;
	QListView *m_listView;
public:
	InputManagerWidget(InputManager *inputManager);
	virtual ~InputManagerWidget();
private slots:
	void showCreator();
};

} // namespace Visualizer

#endif
