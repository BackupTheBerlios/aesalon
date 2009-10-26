#ifndef AESALON_PLATFORM_PIPE_QUEUE_H
#define AESALON_PLATFORM_PIPE_QUEUE_H

#include <queue>
#include <string>

namespace Aesalon {
namespace Platform {

class PipeQueue {
public:
    typedef std::queue<std::string> pipe_queue_t;
private:
    pipe_queue_t pipe_queue;
public:
    PipeQueue() {}
    virtual ~PipeQueue() {}
    
    void add_string(std::string data) { pipe_queue.push(data); }
    std::string pop_string() {
        std::string data = pipe_queue.front();
        pipe_queue.pop();
        return data;
    }
};

}  // namespace Platform
}  // namespace Aesalon

#endif
