#include <iostream>

class BaseClass {
public:
    BaseClass() {}
    virtual ~BaseClass() {}

    virtual int get_data() const = 0;
};

class DataClass : public BaseClass {
private:
    int data;
public:
    DataClass() : data(0) {}
    virtual ~DataClass() {}

    virtual int get_data() const { return data; }
};

int main(int argc, char *argv[]) {
    BaseClass *data = NULL;
    std::cout << "Allocating new DataClass . . . " << std::endl;
    data = new DataClass();
    std::cout << "Allocated data, address is 0x" << std::hex << (unsigned long)data << " . . ." << std::endl;
    delete data;
    std::cout << "Deleted data, exiting . . ." << std::endl;
    return 0;
}
