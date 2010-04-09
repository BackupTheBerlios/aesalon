#include "Parser.h"

namespace Analyzer {

Parser::Parser(int file_fd, File *file) : file_fd(file_fd), file(file) {

}

Parser::~Parser() {

}

} // namespace Analyzer
