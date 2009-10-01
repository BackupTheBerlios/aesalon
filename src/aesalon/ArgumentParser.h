#ifndef AESALON_ARGUMENT_PARSER_H
#define AESALON_ARGUMENT_PARSER_H

#ifndef ARGUMENT_BITSET_SIZE
    #define ARGUMENT_BITSET_SIZE 32
#endif

#include <string>
#include <cstddef>
#include <bitset>
#include <vector>

namespace Aesalon {

class ArgumentPair {
private:
    std::string content;
    char shortform;
    std::size_t position;
public:
    ArgumentPair(std::size_t position, std::string content, char shortform = 0) :
        content(content), shortform(shortform), position(position) {}
    virtual ~ArgumentPair() {}
    
    std::string get_content() const { return content; }
    char get_shortform() const { return shortform; }
    std::size_t get_position() const { return position; }
};

class FileArgument {
private:
    std::string filename;
public:
    FileArgument(std::string filename) : filename(filename) {}
    virtual ~FileArgument() {}
};

class ArgumentParser {
public:
    typedef std::bitset<ARGUMENT_BITSET_SIZE> arguments_bitset_t;
    typedef std::vector<ArgumentPair *> argument_pair_vector_t;
    typedef std::vector<FileArgument *> file_argument_vector_t;
private:
    arguments_bitset_t arguments_bitset;
    argument_pair_vector_t argument_pair_vector;
    file_argument_vector_t file_argument_vector;
    
    void add_file_argument(std::string filename)
        { file_argument_vector.push_back(new FileArgument(filename)); }
public:
    ArgumentParser() {}
    virtual ~ArgumentParser() {}
    
    void add_argument_pair(ArgumentPair *ap)
        { argument_pair_vector.push_back(ap); }
    ArgumentPair *get_argument_pair(std::size_t which) const
        { return argument_pair_vector.at(which); }
    
    arguments_bitset_t &get_bitset() { return arguments_bitset; }
    file_argument_vector_t &get_file_arguments() { return file_argument_vector; }
    
    void parse_arguments(char *argv[]);
};

} // namespace Aesalon

#endif
