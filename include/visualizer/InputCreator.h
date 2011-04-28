/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/InputCreator.h
*/

#ifndef AesalonVisualizer_InputCreator_H
#define AesalonVisualizer_InputCreator_H

#include <QWidget>
#include <QDialog>
#include <QLabel>

#include "DataInput.h"
#include "InputManager.h"

namespace Visualizer {

class InputCreator : public QDialog { Q_OBJECT
private:
	QPushButton *m_logCreateButton;
	QLabel *m_fileLocation;
	InputManager *m_inputManager;
public:
	InputCreator(InputManager *inputManager);
	virtual ~InputCreator();
private:
	QWidget *setupLogTab();
private slots:
	void fileSelected();
	void createLogInput();
};

} // namespace Visualizer

#endif
