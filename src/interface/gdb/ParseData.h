#ifndef AESALON_INTERFACE_GDB_PARSE_DATA_H
#define AESALON_INTERFACE_GDB_PARSE_DATA_H

#include <string>
#include <vector>
#include <map>

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

/* Base class */
class ParseData {
public:
    virtual ~ParseData() {}
};

class ParseString : public ParseData {
private:
    std::string data;
public:
    ParseString(std::string data) : data(data) {}
    virtual ~ParseString() {}
    
    std::string get_data() const { return data; }
};

class ParseResult : public ParseData {
private:
    std::string name;
    Misc::SmartPointer<ParseData> value;
public:
    ParseResult(std::string name, Misc::SmartPointer<ParseData> value) : name(name), value(value) {}
    virtual ~ParseResult() {}
    
    std::string get_name() const { return name; }
    Misc::SmartPointer<ParseData> get_value() const { return value; }
};

class ParseTuple : public ParseData {
public:
    typedef std::map<std::string, Misc::SmartPointer<ParseData> > tuple_map_t;
private:
    tuple_map_t tuple_map;
public:
    virtual ~ParseTuple();
    
    void add_element(Misc::SmartPointer<ParseResult> new_element)
        { tuple_map[new_element->get_name()] = new_element; }
    Misc::SmartPointer<ParseData> get_element(std::string name) const
        { return tuple_map.at(name); }
};

class ParseList : public ParseData {
public:
    typedef std::vector<Misc::SmartPointer<ParseData> > list_vector_t;
private:
    list_vector_t list_vector;
public:
    ParseList() {}
    virtual ~ParseList() {}
    
    void add_element(Misc::SmartPointer<ParseData> new_element) { list_vector.push_back(new_element); }
    Misc::SmartPointer<ParseData> get_element(std::size_t which) const { return list_vector[which]; }
    std::size_t num_elements() const { return list_vector.size(); }
};

class ParseSequence : public ParseTuple {
private:
    std::string first;
public:
    ParseSequence(std::string first) : first(first) {}
    virtual ~ParseSequence() {}
    
    std::string get_first() const { return first; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
