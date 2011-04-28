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
#include <QAbstractTableModel>

#include "DataInput.h"
#include "ArtisanManager.h"

namespace Visualizer {

class InputManager : public QAbstractTableModel { Q_OBJECT
private:
	ArtisanManager *m_artisanManager;
	QList<DataInput *> m_inputList;
public:
	InputManager();
	virtual ~InputManager();
	
	ArtisanManager *artisanManager() const { return m_artisanManager; }
	QList<DataInput *> &inputList() { return m_inputList; }
	
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
public slots:
	void addInput(DataInput *input);
};

} // namespace Visualizer

#endif
