#ifndef AESALON_MONITOR_PTRACE_SIGNAL_OBSERVER_H
#define AESALON_MONITOR_PTRACE_SIGNAL_OBSERVER_H





class SignalObserver {
public:
    SignalObserver() {}
    virtual ~SignalObserver() {}
    
    virtual bool handle_signal(int signal, int status) = 0;
};





#endif
