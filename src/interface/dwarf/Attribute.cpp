#include "Attribute.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

void Attribute::set_data(data_type_e new_type, Word word_data) {
    switch(new_type) {
        case BLOCK_DATA:
        case STRING_DATA:
        case FLAG_DATA:
            throw TypeMismatchException();
        default: {
            set_data_type(new_type);
            internal_data.word_data = word_data;
        }   
    }
}

void Attribute::set_data(data_type_e new_type, Misc::SmartPointer<Byte> block_data) {
    if(new_type != BLOCK_DATA) throw TypeMismatchException();
    internal_data.block_data = block_data;
}

void Attribute::set_data(data_type_e new_type, std::string string_data) {
    if(new_type != STRING_DATA) throw TypeMismatchException();
    internal_data.string_data = string_data;
}
void Attribute::set_data(data_type_e new_type, bool flag_data) {
    if(new_type != FLAG_DATA) throw TypeMismatchException();
    internal_data.flag_data = flag_data;
}

Word Attribute::as_word() const {
    switch(get_data_type()) {
        case BLOCK_DATA:
        case STRING_DATA:
        case FLAG_DATA:
            throw TypeMismatchException();
        default: {
            return internal_data.word_data;
        }   
    }
}
Misc::SmartPointer<Byte> Attribute::as_block() const {
    if(get_data_type() != BLOCK_DATA) throw TypeMismatchException();
    return internal_data.block_data;
}
std::string Attribute::as_string() const {
    if(get_data_type() != STRING_DATA) throw TypeMismatchException();
    return internal_data.string_data;
}
bool Attribute::as_flag() const {
    if(get_data_type() != FLAG_DATA) throw TypeMismatchException();
    return internal_data.flag_data;
}

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon
