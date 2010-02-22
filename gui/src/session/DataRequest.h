#ifndef AESALON_GUI_SESSION_DATA_REQUEST_H
#define AESALON_GUI_SESSION_DATA_REQUEST_H

class DataThread;
class VisualizationData;
class VisualizationThread;

class DataRequest {
private:
    VisualizationThread *v_thread;
public:
    DataRequest(VisualizationThread *v_thread) : v_thread(v_thread) {}
    virtual ~DataRequest() {}
    
    virtual void gather_data(DataThread *data_thread) = 0;
    virtual VisualizationData *create_data() = 0;
    
    VisualizationThread *get_v_thread() const { return v_thread; }
};

#endif
