#include "GraphDataEngine.h"
#include "GraphDataEngine.moc"

GraphDataEngine::GraphDataEngine(QObject *parent) : QObject(parent) {

}

GraphDataEngine::~GraphDataEngine() {
    
}

void GraphDataEngine::handle_requests() {
    DataRequest *request;
    while((request = pop_request())) {
        request->process_response();
        
        delete request;
    }
}
