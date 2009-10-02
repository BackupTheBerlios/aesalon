#ifndef AESALON_DISPLAY_STDOUT_DISPLAY_H
#define AESALON_DISPLAY_STDOUT_DISPLAY_H

#include "../BasicDisplay.h"

namespace Aesalon {
namespace Display {
namespace Stdout {

class StdoutDisplay : public BasicDisplay {
public:
    StdoutDisplay();
    virtual ~StdoutDisplay();
    virtual void start();
};

class ScopeAllocationInformation {
private:
    std::size_t number;
    std::size_t size;
public:
    ScopeAllocationInformation(std::size_t number, std::size_t size) :
        number(number), size(size){}
    virtual ~ScopeAllocationInformation() {}
    
    void inc_number() { number ++; }
    std::size_t get_number() const { return number; }
    void add_size(std::size_t size) { this->size += size; }
    std::size_t get_size() const { return size; }
};

} // namespace Stdout
} // namespace Display
} // namespace Aesalon


#endif
