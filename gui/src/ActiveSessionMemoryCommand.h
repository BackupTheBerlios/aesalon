#ifndef AESALON_GUI_ACTIVE_SESION_MEMORY_COMMAND_H
#define AESALON_GUI_ACTIVE_SESION_MEMORY_COMMAND_H

#include <QQueue>
#include <QDateTime>

class ActiveSessionMemorySnapshot;

class ActiveSessionMemoryCommand {
private:
    QDateTime associated_time;
public:
    enum command_type_e {
        SNAPSHOT,
        BLOCK
    };
    ActiveSessionMemoryCommand(QDateTime associated_time = QDateTime::currentDateTime())
        : associated_time(associated_time) {}
    virtual ~ActiveSessionMemoryCommand() {}
    
    virtual void apply_to(ActiveSessionMemorySnapshot *memory) = 0;
    virtual void unapply_from(ActiveSessionMemorySnapshot *memory) = 0;
    
    const QDateTime &get_associated_time() const { return associated_time; }
    void set_associated_time(const QDateTime &new_time) { associated_time = new_time; }
    
    /*virtual command_type_e get_type() const = 0;*/
};

class ActiveSessionMemoryCommandQueue : public QQueue<ActiveSessionMemoryCommand *> {
public:
    ActiveSessionMemoryCommandQueue() : QQueue< ActiveSessionMemoryCommand* >() {}
    virtual ~ActiveSessionMemoryCommandQueue() {}
};

#endif
