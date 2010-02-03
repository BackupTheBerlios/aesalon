#ifndef AESALON_ACITVE_SESSION_MEMORY_H
#define AESALON_ACITVE_SESSION_MEMORY_H

#include <QObject>
#include <QDateTime>

#include <QList>

#include "ActiveSessionMemoryStorage.h"

class ActiveSessionMemorySnapshot {
private:
    quint64 allocations, deallocations, reallocations;
public:
    ActiveSessionMemorySnapshot() : allocations(0), deallocations(0), reallocations(0) {}
    ~ActiveSessionMemorySnapshot() {}
    
    quint64 get_allocations() const { return allocations; }
    void set_allocations(quint64 new_allocations) { allocations = new_allocations; }
    quint64 get_deallocations() const { return deallocations; }
    void set_deallocations(quint64 new_deallocations) { deallocations = new_deallocations; }
    quint64 get_reallocations() const { return reallocations; }
    void set_reallocations(quint64 new_reallocations) { reallocations = new_reallocations; }
};

class ActiveSessionMemory : public QObject { Q_OBJECT
private:
    QByteArray unprocessed;
    quint64 pop_uint64();
    
    /** The place where the memory blocks and snapshots are stored. */
    ActiveSessionMemoryStorage *storage;
    
    /** The current status of the program's memory. */
    ActiveSessionMemorySnapshot *current_memory;
    /** The differences since the last pushed snapshot. */
    ActiveSessionMemorySnapshot *current_changes;
public:
    ActiveSessionMemory(QObject *parent = NULL);
    virtual ~ActiveSessionMemory();
    
    ActiveSessionMemorySnapshot *get_current_memory() const { return current_memory; }
    /** Returns a snapshot of all of the events up until a given time.
        @param time The time to search for.
        @return A new snapshot, consisting of the memory at the given time. The caller
            is required to free this object.
    */
    ActiveSessionMemorySnapshot *get_snapshot_for(QDateTime time) const { return NULL; }
public slots:
    void process_data(QByteArray data);
signals:
    void memory_changed(ActiveSessionMemorySnapshot *current_snapshot);
};

#endif
