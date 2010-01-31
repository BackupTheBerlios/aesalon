#ifndef AESALON_GUI_ACTIVE_SESSION_MEMORY_BLOCK_H
#define AESALON_GUI_ACTIVE_SESSION_MEMORY_BLOCK_H

#include <QtGlobal>
#include <QDateTime>

#include "ActiveSessionMemoryCommand.h"

class ActiveSessionMemoryBlock {
private:
    quint64 address;
    quint64 size;
public:
    ActiveSessionMemoryBlock(quint64 address, quint64 size) : address(address), size(size) {}
    virtual ~ActiveSessionMemoryBlock() {}
    
    quint64 get_address() const { return address; }
    void set_address(quint64 new_address) { address = new_address; }
    quint64 get_size() const { return size; }
    void set_size(quint64 new_size) { size = new_size; }
};

class ActiveSessionMemoryBlockAllocation : public ActiveSessionMemoryCommand {
private:
    quint64 address, size;
public:
    ActiveSessionMemoryBlockAllocation(QDateTime associated_time, quint64 address, quint64 size)
        : ActiveSessionMemoryCommand(associated_time), address(address), size(size) {}
    virtual ~ActiveSessionMemoryBlockAllocation() {}
    
    virtual void apply_to(ActiveSessionMemorySnapshot *memory);
    virtual void unapply_from(ActiveSessionMemorySnapshot *memory);
};

class ActiveSessionMemoryBlockDeallocation : public ActiveSessionMemoryCommand {
private:
    quint64 address;
    quint64 old_size;
public:
    ActiveSessionMemoryBlockDeallocation(QDateTime associated_time, quint64 address, quint64 old_size)
        : ActiveSessionMemoryCommand(associated_time), address(address), old_size(old_size) {}
    virtual ~ActiveSessionMemoryBlockDeallocation() {}
    
    virtual void apply_to(ActiveSessionMemorySnapshot *memory);
    virtual void unapply_from(ActiveSessionMemorySnapshot *memory);
};

class ActiveSessionMemoryBlockReallocation : public ActiveSessionMemoryCommand {
private:
    quint64 old_address;
    quint64 new_address;
    quint64 old_size;
    quint64 new_size;
public:
    ActiveSessionMemoryBlockReallocation(QDateTime associated_time, quint64 old_address, quint64 new_address, quint64 old_size, quint64 new_size)
        : ActiveSessionMemoryCommand(associated_time), old_address(old_address), new_address(new_address), old_size(old_size), new_size(new_size) {}
    virtual ~ActiveSessionMemoryBlockReallocation() {}
    
    virtual void apply_to(ActiveSessionMemorySnapshot *memory);
    virtual void unapply_from(ActiveSessionMemorySnapshot *memory);
};

#endif
