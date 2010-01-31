#include <QTimer>

#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

ActiveSessionMemorySnapshot *ActiveSessionMemorySnapshot::clone() const {
    ActiveSessionMemorySnapshot *ret = new ActiveSessionMemorySnapshot();
    for(block_list_t::const_iterator i = block_list.begin(); i != block_list.end(); i ++) {
        ret->add_block(new ActiveSessionMemoryBlock((*i)->get_address(), (*i)->get_size()));
    }
    ret->set_associated_time(get_associated_time());
    return ret;
}

void ActiveSessionMemorySnapshot::add_block(ActiveSessionMemoryBlock *block) {
    block_list.append(block);
}

ActiveSessionMemoryBlock *ActiveSessionMemorySnapshot::get_block(quint64 address) const {
    for(block_list_t::const_iterator i = block_list.begin(); i != block_list.end(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    return NULL;
}

void ActiveSessionMemorySnapshot::remove_block(quint64 address) {
    for(block_list_t::const_iterator i = block_list.begin(); i != block_list.end(); i ++) {
        if((*i)->get_address() == address) {
            block_list.removeOne(*i);
            return;
        }
    }
    qWarning("Warning: Asked to remove unknown block (address is %ul) from snapshot, block list size is %i", address, block_list.size());
}

void ActiveSessionMemorySnapshot::apply_to(ActiveSessionMemorySnapshot *memory) {
    memory->block_list.clear();
    for(block_list_t::const_iterator i = block_list.begin(); i != block_list.end(); i ++) {
        memory->add_block(new ActiveSessionMemoryBlock((*i)->get_address(), (*i)->get_size()));
    }
    memory->set_associated_time(get_associated_time());
}

void ActiveSessionMemorySnapshot::unapply_from(ActiveSessionMemorySnapshot *memory) {
    qWarning("Warning: Asked to unapply snapshot from snapshot");
}

ActiveSessionMemorySnapshot *ActiveSessionMemorySnapshot::find_changed(ActiveSessionMemorySnapshot *from) {
    ActiveSessionMemorySnapshot *ret = new ActiveSessionMemorySnapshot();
    
    for(block_list_t::const_iterator i = block_list.begin(); i != block_list.end(); i ++) {
        ActiveSessionMemoryBlock *block = from->get_block((*i)->get_address());
        if(!block || block->get_size() != (*i)->get_size()) ret->add_block(*i);
    }
    return ret;
}

ActiveSessionMemorySnapshot *ActiveSessionMemorySnapshot::find_removed(ActiveSessionMemorySnapshot *from) {
    ActiveSessionMemorySnapshot *ret = new ActiveSessionMemorySnapshot();
    
    for(block_list_t::const_iterator i = from->block_list.begin(); i != from->block_list.end(); i ++) {
        if(get_block((*i)->get_address()) == NULL) ret->add_block(*i);
    }
    return ret;
}

ActiveSessionMemory::ActiveSessionMemory(QObject *parent) : QObject(parent), current_snapshot(NULL) {
    current_snapshot = new ActiveSessionMemorySnapshot();
}

quint64 ActiveSessionMemory::pop_uint64() {
    if(unprocessed.size() < 8) return -1;
    QByteArray data = unprocessed.left(8);
    unprocessed.remove(0, 8);
    quint64 value = 0;
    /* NOTE: this is an unrolled loop, doesn't have to be like this. */
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
    while(unprocessed.size()) {
        ActiveSessionMemoryCommand *command = NULL;
        char type = unprocessed[0];
        unprocessed.remove(0, 1);
        if(type & 0x01) {
            /* block event */
            quint64 address = 0;
            int block_type = (type & 0x06) >> 1;
            address = pop_uint64();
            /* block type is zero for allocations */
            if(block_type == 0) {
                quint64 size = pop_uint64();
                command = new ActiveSessionMemoryBlockAllocation(QDateTime::currentDateTime(), address, size);
            }
            /* block type is one for reallocations */
            else if(block_type == 1) {
                quint64 new_size = pop_uint64();
                quint64 new_address = pop_uint64();
                ActiveSessionMemoryBlock *block = get_current_snapshot()->get_block(address);
                if(!block) continue;
                command = new ActiveSessionMemoryBlockReallocation(QDateTime::currentDateTime(), address, new_address, block->get_size(), new_size);
            }
            /* block type is two for deallocations */
            else if(block_type == 2) {
                ActiveSessionMemoryBlock *block = get_current_snapshot()->get_block(address);
                if(!block) continue;
                command = new ActiveSessionMemoryBlockDeallocation(QDateTime::currentDateTime(), address, block->get_size());
            }
            /* block type three is unused, also known as an error . . . */
        }
        if(command == NULL) continue;
        /* Now, apply it to the current snapshot . . . */
        command->apply_to(get_current_snapshot());
        /* And push it onto the current-event queue. */
        current_queue.enqueue(command);
    }
    /* If it's more than 1000 events on the queue, then push the current snapshot onto the snapshot queue and clear the event queue . . . */
    if(current_queue.size() > 1000) {
        snapshot_queue.enqueue(get_current_snapshot()->clone());
        current_queue.clear();
    }
    /* Store a maximum of 10 snapshots on the snapshot queue . . . */
    if(snapshot_queue.size() >= 10) {
        ActiveSessionMemorySnapshot *snapshot = new ActiveSessionMemorySnapshot();
        snapshot_queue[snapshot_queue.size()-1]->apply_to(snapshot);
        snapshot_queue.clear();
        unbounded_snapshot_queue.enqueue(snapshot);
    }
    emit memory_changed(get_current_snapshot());
}

ActiveSessionMemorySnapshot *ActiveSessionMemory::get_snapshot_for(QDateTime time) const {
    ActiveSessionMemoryCommand *command = NULL;
    for(int x = 0; x < snapshot_queue.size(); x ++) {
        if(time <= snapshot_queue[x]->get_associated_time()) command = snapshot_queue[x];
        else break;
    }
    if(command) return (dynamic_cast<ActiveSessionMemorySnapshot *>(command))->clone();
    
    ActiveSessionMemorySnapshot *snapshot = new ActiveSessionMemorySnapshot();
    for(int x = 0; x < current_queue.size(); x ++) {
        if(time <= current_queue[x]->get_associated_time()) current_queue[x]->apply_to(snapshot);
        else break;
    }
    
    return snapshot;
}
