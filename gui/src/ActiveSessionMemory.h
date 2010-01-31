#ifndef AESALON_ACITVE_SESSION_MEMORY_H
#define AESALON_ACITVE_SESSION_MEMORY_H

#include <QObject>
#include <QDateTime>

#include <QList>

#include "ActiveSessionMemoryBlock.h"
#include "ActiveSessionMemoryCommand.h"

class ActiveSessionMemorySnapshot : public ActiveSessionMemoryCommand {
private:
    typedef QList<ActiveSessionMemoryBlock *> block_list_t;
    
    block_list_t block_list;
    
    quint64 allocations, deallocations, reallocations;
public:
    ActiveSessionMemorySnapshot() : allocations(0), deallocations(0), reallocations(0) {}
    virtual ~ActiveSessionMemorySnapshot() {}
    
    ActiveSessionMemorySnapshot *clone() const;
    
    virtual void apply_to(ActiveSessionMemorySnapshot *memory);
    virtual void unapply_from(ActiveSessionMemorySnapshot *memory);
    
    void add_block(ActiveSessionMemoryBlock *block);
    ActiveSessionMemoryBlock *get_block(quint64 address) const;
    ActiveSessionMemoryBlock *get_block_by_index(int i) const { return block_list[i]; }
    void remove_block(quint64 address);
    int get_blocks() const { return block_list.size(); }
    
    quint64 get_allocations() const { return allocations; }
    void set_allocations(quint64 new_allocations) { allocations = new_allocations; }
    quint64 get_deallocations() const { return deallocations; }
    void set_deallocations(quint64 new_deallocations) { deallocations = new_deallocations; }
    quint64 get_reallocations() const { return reallocations; }
    void set_reallocations(quint64 new_reallocations) { reallocations = new_reallocations; }
    
    /** Compares the current snapshot to another, and returns a new snapshot with
        the blocks that were added or resized between @a from and this.
        @param from The snapshot to compare against.
        @return The added blocks between @a other and this. The caller is responsible
            for freeing this object, and must not change the blocks within.
    */
    ActiveSessionMemorySnapshot *find_changed(ActiveSessionMemorySnapshot *from);
    
    /** Compares the current snapshot to another, and returns a new snapshot with
        the blocks that were removed between @a from and this.
        @param from The snapshot to compare against.
        @return The added blocks between @a other and this. The caller is responsible
            for freeing this object, and must not change the blocks within.
    */
    ActiveSessionMemorySnapshot *find_removed(ActiveSessionMemorySnapshot *from);
    
    
    /*virtual command_type_e get_type() const { return SNAPSHOT; }*/
};

class ActiveSessionMemory : public QObject { Q_OBJECT
private:
    QByteArray unprocessed;
    quint64 pop_uint64();
    
    /** The current status of the program's memory. */
    ActiveSessionMemorySnapshot *current_snapshot;
    /** This command queue stores the short-term, raw Allocation etc. information. 
        Once a certain number of commands have been pushed on, a snapshot is generated
        and pushed onto @a snapshot_queue, and this queue is emptied. */
    ActiveSessionMemoryCommandQueue current_queue;
    /** Collections of a few hundred commands, when this queue reaches a certain size,
        a snapshot is copied into the @a unbounded_snapshot_queue.
    */
    ActiveSessionMemoryCommandQueue snapshot_queue;
    /** The long-term list of snapshots. Most of the memory taken by the GUI will be
        in this queue.
    */
    ActiveSessionMemoryCommandQueue unbounded_snapshot_queue;
public:
    ActiveSessionMemory(QObject *parent = NULL);
    virtual ~ActiveSessionMemory() {}
    
    ActiveSessionMemorySnapshot *get_current_snapshot() const { return current_snapshot; }
public slots:
    void process_data(QByteArray data);
signals:
    void memory_changed(ActiveSessionMemorySnapshot *current_snapshot);
};

#endif
