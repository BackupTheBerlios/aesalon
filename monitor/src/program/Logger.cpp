#include <string>
#include <fcntl.h>
#include <unistd.h>

#include "Logger.h"
#include "Initializer.h"

namespace Program {

Logger::Logger() : m_fd(0) {
	std::string filename = Initializer::singleton()->configuration()->traverse("logFile")->data();
	
	if(!filename.length()) return;
	
	if(filename != "-") m_fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	else m_fd = STDOUT_FILENO;
}

Logger::~Logger() {
	close(m_fd);
}

void Logger::logPacket(DataPacket *packet) {
	if(m_fd == 0) return;
	write(m_fd, &packet->dataSource, sizeof(packet->dataSource));
	write(m_fd, &packet->dataSize, sizeof(packet->dataSize));
	write(m_fd, packet->data, packet->dataSize);
}

} // namespace Program
