#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

ActiveSessionMemory::ActiveSessionMemory(QObject *parent) : QObject(parent) {
    
}

void ActiveSessionMemory::process_data(QByteArray data) {
    unprocessed += data;
    qDebug("Amount of unprocessed data: %i, increased by %i", unprocessed.size(), data.size());
}
