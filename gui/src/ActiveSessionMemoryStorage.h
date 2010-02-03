#ifndef AESALON_GUI_ACTIVE_SESSION_MEMORY_STORAGE_H
#define AESALON_GUI_ACTIVE_SESSION_MEMORY_STORAGE_H

#include <QtGlobal>
#include <QMap>

class ActiveSessionMemoryBlock;
class ActiveSessionMemorySnapshot;

/** The type used for storing data offsets. This MUST be a signed type, as
    negative offsets in Snapshots indicate item removals. */
typedef qint64 StorageOffset;

class ActiveSessionMemoryStorage {
public:
    enum allocation_mode_e {
        ALLOC_MODE_1M,
        ALLOC_MODE_DOUBLE
    };
private:
    enum data_type_e {
        INVALID_DATA,
        BLOCK_DATA,
        SNAPSHOT_DATA
    };

    quint8 *data;
    StorageOffset data_size;
    StorageOffset unused_offset;
    
    allocation_mode_e allocation_mode;
    
    /** A QMap is used to store the type of each StorageOffset. 
        QMap::operator[] is O(log n) for accessing, so it should be fast enough.
    */
    QMap<StorageOffset, data_type_e> data_types;
    
    /** Reserves more memory. Uses the current allocation_mode to determine how much more to allocate. */
    void reserve_more_memory();
public:
    ActiveSessionMemoryStorage(allocation_mode_e allocation_mode = ALLOC_MODE_1M);
    virtual ~ActiveSessionMemoryStorage();
    
    StorageOffset get_data_size() const { return data_size; }
    
    ActiveSessionMemoryBlock *alloc_new_block(quint64 address, quint64 size);
    ActiveSessionMemorySnapshot *alloc_new_snapshot();
    
    /** Returns the block at @a offset data offset.
        @param offset The offset to look for a block at.
        @return The block at @a offset, or NULL otherwise.
    */
    ActiveSessionMemoryBlock *get_block_at(StorageOffset offset);
    /** Returns the snapshot at @a offset data offset.
        @param offset The offset to look for a snapshot at.
        @return The snapshot at @a offset, or NULL otherwise.
    */
    ActiveSessionMemorySnapshot *get_snapshot_at(StorageOffset offset);
};

#endif
