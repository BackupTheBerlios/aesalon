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
#include <QHash>

#include <stdint.h>

#include "comm/Packet.h"
#include "ArtisanWrapper.h"
#include "ArtisanManager.h"

namespace Visualizer {

class DataInput : public QThread { Q_OBJECT
private:
	QByteArray m_unprocessed;
	QHash<uint32_t, uint32_t> m_processIDMap;
	ArtisanManager *m_artisanManager;
	QHash<ModuleID, ArtisanWrapper *> m_artisanMap;
	bool m_enabled;
public:
	DataInput(ArtisanManager *artisanManager);
	virtual ~DataInput() {}
	
	virtual QString title() const { return "<default>"; }
	
	bool isEnabled() const { return m_enabled; }
	void disable() { m_enabled = false; }
	void enable() { m_enabled = true; }
public slots:
	void addData(QByteArray data);
private:
	void processInformerPacket(Comm::Packet *packet);
};

} // namespace Visualizer

#endif
