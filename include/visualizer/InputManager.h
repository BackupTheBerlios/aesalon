/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/InputManager.h
*/

#ifndef AesalonVisualizer_InputManager_H
#define AesalonVisualizer_InputManager_H

#include <QObject>
#include <QList>

#include "DataInput.h"

namespace Visualizer {

class InputManager : public QObject { Q_OBJECT
private:
	QList<DataInput *> m_inputList;
public:
	InputManager();
	virtual ~InputManager();
	
	QList<DataInput *> &inputList() { return m_inputList; }
public slots:
	void addInput(DataInput *input);
signals:
	void inputAdded(DataInput *input);
};

} // namespace Visualizer

#endif
