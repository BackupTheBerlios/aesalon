#include "ActiveSessionMemoryBlock.h"
#include "ActiveSessionMemory.h"

void ActiveSessionMemoryBlockAllocation::apply_to(ActiveSessionMemorySnapshot *memory) {
    memory->add_block(new ActiveSessionMemoryBlock(address, size));
    memory->set_allocations(memory->get_allocations()+1);
    memory->set_associated_time(get_associated_time());
}

void ActiveSessionMemoryBlockAllocation::unapply_from(ActiveSessionMemorySnapshot *memory) {
    memory->remove_block(address);
    memory->set_allocations(memory->get_allocations()-1);
}

void ActiveSessionMemoryBlockDeallocation::apply_to(ActiveSessionMemorySnapshot *memory) {
    memory->remove_block(address);
    memory->set_deallocations(memory->get_deallocations()+1);
    memory->set_associated_time(get_associated_time());
}

void ActiveSessionMemoryBlockDeallocation::unapply_from(ActiveSessionMemorySnapshot *memory) {
    memory->add_block(new ActiveSessionMemoryBlock(address, old_size));
    memory->set_deallocations(memory->get_deallocations()-1);
}

void ActiveSessionMemoryBlockReallocation::apply_to(ActiveSessionMemorySnapshot *memory) {
    if(old_address == new_address) {
        memory->get_block(old_address)->set_size(new_size);
    }
    else {
        memory->remove_block(old_address);
        memory->add_block(new ActiveSessionMemoryBlock(new_address, new_size));
    }
    memory->set_reallocations(memory->get_reallocations()+1);
    memory->set_associated_time(get_associated_time());
}

void ActiveSessionMemoryBlockReallocation::unapply_from(ActiveSessionMemorySnapshot *memory) {
    if(old_address == new_address) {
        memory->get_block(new_address)->set_size(old_size);
    }
    else {
        memory->remove_block(new_address);
        memory->add_block(new ActiveSessionMemoryBlock(old_address, old_size));
    }
    memory->set_reallocations(memory->get_reallocations()-1);
}
