#ifndef AESALON_OVERLOAD_PARSER_H
#define AESALON_OVERLOAD_PARSER_H

class OverloadParser {
private:
    int pipe_fd;
public:
    OverloadParser(int pipe_fd);
    ~OverloadParser();
    
    void parse();
};

#endif
