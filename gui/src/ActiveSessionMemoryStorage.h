#ifndef AESALON_GUI_ACTIVE_SESSION_MEMORY_STORAGE_H
#define AESALON_GUI_ACTIVE_SESSION_MEMORY_STORAGE_H

#include <QtGlobal>

class ActiveSessionMemoryStorage {
public:
    enum allocation_mode_e {
        ALLOC_MODE_1M,
        ALLOC_MODE_DOUBLE
    };
private:
    quint8 *data;
    quint64 data_size;
    quint64 unused_offset;
    
    allocation_mode_e allocation_mode;
    
    /** Reserves more memory. Uses the current allocation_mode to determine how much more to allocate.
    */
    void reserve_more_memory();
public:
    ActiveSessionMemoryStorage(allocation_mode_e allocation_mode = ALLOC_MODE_1M);
    virtual ~ActiveSessionMemoryStorage();
    
    quint64 get_data_size() const { return data_size; }
};

#endif
