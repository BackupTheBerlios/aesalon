#include "GraphDataEngine.h"
#include "GraphDataEngine.moc"
#include "RenderThread.h"

GraphDataEngine::GraphDataEngine(QObject *parent, DataThread *data_thread) : QObject(parent), data_thread(data_thread) {
    
}

GraphDataEngine::~GraphDataEngine() {
    
}

void GraphDataEngine::request_data(RenderThread *thread, const Timestamp &timestamp) {
    /* TODO: cache the result . . . */
    data_thread->push_request(spawn_new_request(timestamp));
}

void GraphDataEngine::handle_requests() {
    DataRequest *request;
    while((request = pop_request())) {
        request->process_response();
        delete request;
    }
}
