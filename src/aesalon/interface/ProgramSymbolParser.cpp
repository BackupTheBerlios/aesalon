#include "ProgramSymbolParser.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

namespace Aesalon {
namespace Interface {

void ProgramSymbolParser::parse_line(std::string line) {
    std::string symbol_name;
    std::size_t symbol_address;
    
    std::stringstream ss;
    ss << line;
    ss >> std::hex >> symbol_address;
    /* to get rid of the middle column */
    ss >> symbol_name;
    ss >> symbol_name;
    
    ProgramSymbol *ps;
    ps = new ProgramSymbol(symbol_name, symbol_address);
    std::cout << "Constructed new ProgramSymbol . . ." << std::endl;
    std::vector< Misc::SmartPointer<ProgramSymbol> > sv_test;
    symbol_vector.push_back(ps);
}

void ProgramSymbolParser::parse() {
    int pipe_fd[2];
    pipe(pipe_fd);
    
    std::cout << "Beginning parse of nm . . ." << std::endl;
    
    if(fork() == 0) {
        /* Don't need to read from the pipe, output only */
        close(pipe_fd[0]);
        fcntl(pipe_fd[1], F_SETFL, fcntl(pipe_fd[1], F_GETFL) & ~O_NONBLOCK);
        dup2(pipe_fd[1], STDOUT_FILENO); /* Reassign stdout . . .*/
        execl("/usr/bin/nm", "/usr/bin/nm", get_filename().c_str(), 0);
    }
    
    wait(NULL);
    
    std::cout << "Parsing output of nm . . ." << std::endl;
    
    std::string line;
    char buffer;
    while(read(pipe_fd[0], &buffer, sizeof(char)) > 0) {
        if(buffer != '\n') line += buffer;
        else {
            parse_line(line);
            line.clear();
        }
    }
}

std::string ProgramSymbolParser::find_name_by_address(std::size_t address) {
    std::size_t x = 0;
    if(x > symbol_vector.size()) return "";
    while(get_address_by_number(x) && get_address_by_number(x) < address) x ++;
    return symbol_vector[x]->get_symbol_name();
}

} // namespace Interface
} // namespace Aesalon
