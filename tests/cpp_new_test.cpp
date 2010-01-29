#include <iostream>

int main(int argc, char *argv[]) {
    char *data = NULL;
    std::cout << "Allocating data, size 16 . . . " << std::endl;
    data = new char[16];
    std::cout << "Allocated data, address is 0x" << std::hex << (unsigned long)data << " . . ." << std::endl;
    delete data;
    std::cout << "Deleted data, exiting . . ." << std::endl;
    return 0;
}
