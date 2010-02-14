#include "GraphDataEngine.h"
#include "GraphDataEngine.moc"

GraphDataEngine::GraphDataEngine(QObject *parent, DataThread *data_thread) : QObject(parent), data_thread(data_thread) {
    
}

GraphDataEngine::~GraphDataEngine() {
    
}

void GraphDataEngine::get_data(const Timestamp &timestamp) {
    data_thread->push_request(spawn_new_request(timestamp));
}

void GraphDataEngine::handle_requests() {
    DataRequest *request;
    while((request = pop_request())) {
        request->process_response();
        delete request;
    }
}
