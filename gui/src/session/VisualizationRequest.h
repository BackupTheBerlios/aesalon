#ifndef AESALON_GUI_SESSION_VISUALIZATION_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_REQUEST_H

#include "storage/Timestamp.h"
#include "DataRequest.h"

class VisualizationRequest {
private:
    Timestamp from, to;
public:
    VisualizationRequest(const Timestamp &from, const Timestamp &to) : from(from), to(to) {}
    
    const Timestamp &get_from() const { return from; }
    const Timestamp &get_to() const { return to; }
};

#endif
