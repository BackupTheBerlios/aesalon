#ifndef AESALON_ACITVE_SESSION_MEMORY_H
#define AESALON_ACITVE_SESSION_MEMORY_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include <QList>
#include <QSet>

#include "Session.h"
#include "ActiveSessionMemoryStorage.h"

class ActiveSessionMemoryBlock {
private:
    StorageOffset offset;
    quint64 address, size;
public:
    ActiveSessionMemoryBlock(StorageOffset offset, quint64 address, quint64 size)
        : offset(offset), address(address), size(size) {}
    ~ActiveSessionMemoryBlock() {}
    
    StorageOffset get_offset() const { return offset; }
    quint64 get_address() const { return address; }
    quint64 get_size() const { return size; }
};

class ActiveSessionMemorySnapshot {
private:
    ActiveSessionMemoryStorage *storage;
    StorageOffset offset;
    QList<StorageOffset> content;
    quint64 allocations, deallocations, reallocations;
    QDateTime timestamp;
public:
    ActiveSessionMemorySnapshot(ActiveSessionMemoryStorage *storage, StorageOffset offset, QDateTime timestamp)
        : storage(storage), offset(offset), allocations(0), deallocations(0), reallocations(0), timestamp(timestamp) {}
    ~ActiveSessionMemorySnapshot() {}
    
    ActiveSessionMemoryStorage *get_storage() const { return storage; }
    
    StorageOffset get_offset() const { return offset; }
    
    QDateTime get_timestamp() const { return timestamp; }
    void set_timestamp(QDateTime new_timestamp) { timestamp = new_timestamp; }
    
    void add_block(quint64 address, quint64 size);
    void add_block(ActiveSessionMemoryBlock *block);
    void add_block(StorageOffset offset);
    ActiveSessionMemoryBlock *get_block(quint64 address) const;
    void remove_block(ActiveSessionMemoryBlock *block);
    
    QList<StorageOffset> get_content() const { return content; }
    
    quint64 get_allocations() const { return allocations; }
    void set_allocations(quint64 new_allocations) { allocations = new_allocations; }
    void inc_allocations() { allocations ++; }
    quint64 get_deallocations() const { return deallocations; }
    void set_deallocations(quint64 new_deallocations) { deallocations = new_deallocations; }
    void inc_deallocations() { deallocations ++; }
    quint64 get_reallocations() const { return reallocations; }
    void set_reallocations(quint64 new_reallocations) { reallocations = new_reallocations; }
    void inc_reallocations() { reallocations ++; }
    
    void copy_into(ActiveSessionMemorySnapshot *snapshot) const;
    void assemble_from(ActiveSessionMemorySnapshot *snapshot, bool remove = false);
    
    void dump_content();
};

class ActiveSessionMemory : public QObject { Q_OBJECT
private:
    Session *session;
    QByteArray unprocessed;
    quint64 pop_uint64();
    
    /** The place where the memory blocks and snapshots are stored. */
    ActiveSessionMemoryStorage *storage;
    
    /** The current status of the program's memory. */
    ActiveSessionMemorySnapshot *current_memory;
    /** The differences since the last pushed snapshot. */
    ActiveSessionMemorySnapshot *current_changes;
    
    QTimer *snapshot_timer;
    QList<StorageOffset> snapshot_list;
public:
    ActiveSessionMemory(QObject *parent, Session *session);
    virtual ~ActiveSessionMemory();
    
    ActiveSessionMemoryStorage *get_storage() const { return storage; }
    
    ActiveSessionMemorySnapshot *get_current_memory() const { return current_memory; }
    /** Returns a snapshot of all of the events up until a given time.
        @param time The time to search for.
        @return A new snapshot, consisting of the memory at the given time. The caller
            is required to free this object.
    */
    ActiveSessionMemorySnapshot *get_snapshot_for(QDateTime time) const;
private slots:
    void save_snapshot();
public slots:
    void process_data(QByteArray data);
    void started(QDateTime time);
    void finished(QDateTime time);
signals:
    void memory_changed(ActiveSessionMemorySnapshot *current_snapshot);
};

#endif
