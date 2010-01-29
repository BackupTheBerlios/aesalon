#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

ActiveSessionMemory::ActiveSessionMemory(QObject *parent) : QObject(parent) {
    
}

void ActiveSessionMemory::process_data(QByteArray data) {
    unprocessed += data;
    qDebug("Amount of unprocessed data: %i, increased by %i", unprocessed.size(), data.size());
    while(unprocessed.size()) {
        char type = unprocessed[0];
        unprocessed.remove(0, 1);
        if(type & 0x01) {
            /* block event */
            QByteArray address_array, size_array, new_address_array;
            quint64 address, size, new_address;
            int block_type = (type & 0x06) >> 1;
            address_array = unprocessed.left(8);
            unprocessed.remove(0, 8);
            if(block_type == ActiveSessionMemoryBlockDiff::ALLOC_DIFF) {
                size_array = unprocessed.left(8);
                unprocessed.remove(0, 8);
                for(int x = 0; x < 8; x ++) {
                    address |= address_array[7-x];
                    size |= size_array[7-x];
                    address << 8;
                    size << 8;
                }
            }
            else if(block_type == ActiveSessionMemoryBlockDiff::FREE_DIFF) {
                for(int x = 0; x < 8; x ++) {
                    address |= address_array[7-x];
                    address << 8;
                }
            }
            else if(block_type == ActiveSessionMemoryBlockDiff::REALLOC_DIFF) {
                size_array = unprocessed.left(8);
                unprocessed.remove(0, 8);
                new_address_array = unprocessed.left(8);
                unprocessed.remove(0, 8);
                for(int x = 0; x < 8; x ++) {
                    address |= address_array[7-x];
                    size |= size_array[7-x];
                    new_address |= new_address_array[7-x];
                    address << 8;
                    size << 8;
                    new_address << 8;
                }
            }
            ActiveSessionMemoryBlockDiff *diff =
                new ActiveSessionMemoryBlockDiff(this, (ActiveSessionMemoryBlockDiff::diff_type_e)block_type,
                QDateTime::currentDateTime(), address, size, new_address);
        }
        /* unknown event, pop it off for now . . . */
        unprocessed.remove(0, 1);
    }
}
