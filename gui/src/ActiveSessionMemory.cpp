#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

ActiveSessionMemory::ActiveSessionMemory(QObject *parent) : QObject(parent) {
    
}

quint64 ActiveSessionMemory::pop_uint64() {
    if(unprocessed.size() < 8) return -1;
    QByteArray data = unprocessed.left(8);
    unprocessed.remove(0, 8);
    quint64 value = 0;
    value |= (uchar)data[7];
    value <<= 8;
    value |= (uchar)data[6];
    value <<= 8;
    value |= (uchar)data[5];
    value <<= 8;
    value |= (uchar)data[4];
    value <<= 8;
    value |= (uchar)data[3];
    value <<= 8;
    value |= (uchar)data[2];
    value <<= 8;
    value |= (uchar)data[1];
    value <<= 8;
    value |= (uchar)data[0];
    return value;
}

void ActiveSessionMemory::process_data(QByteArray data) {
    unprocessed += data;
    qDebug("Amount of unprocessed data: %i, increased by %i", unprocessed.size(), data.size());
    while(unprocessed.size()) {
        char type = unprocessed[0];
        unprocessed.remove(0, 1);
        if(type & 0x01) {
            /* block event */
            quint64 address = 0, size = 0, new_address = 0;
            int block_type = (type & 0x06) >> 1;
            address = pop_uint64();
            if(block_type == ActiveSessionMemoryBlockDiff::ALLOC_DIFF) {
                size = pop_uint64();
            }
            else if(block_type == ActiveSessionMemoryBlockDiff::REALLOC_DIFF) {
                size = pop_uint64();
                new_address = pop_uint64();
            }
            ActiveSessionMemoryBlockDiff *diff =
                new ActiveSessionMemoryBlockDiff(this, (ActiveSessionMemoryBlockDiff::diff_type_e)block_type,
                QDateTime::currentDateTime(), address, size, new_address);
            qDebug("Processed new BlockDiff, type is %i, address is %p, size is %i, new_address is %p", block_type, address, size, new_address);
            emit diff_processed(diff);
        }
        /* unknown event, pop it off for now . . . */
        unprocessed.remove(0, 1);
    }
}
