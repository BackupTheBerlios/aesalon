/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/DataInput.h
*/

#ifndef AesalonVisualizer_DataInput_H
#define AesalonVisualizer_DataInput_H

#include <QThread>
#include <QByteArray>

namespace Visualizer {

class DataInput : public QThread { Q_OBJECT
private:
	QByteArray m_unprocessed;
public:
	DataInput() {}
	virtual ~DataInput() {}
public slots:
	void addData(QByteArray &data);
};

} // namespace Visualizer

#endif
