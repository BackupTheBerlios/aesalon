#ifndef AESALON_ACITVE_SESSION_MEMORY_H
#define AESALON_ACITVE_SESSION_MEMORY_H

#include <QObject>
#include <QDateTime>

class ActiveSessionMemoryBlockDiff : public QObject { Q_OBJECT
public:
    enum diff_type_e {
        ALLOC_DIFF,
        REALLOC_DIFF,
        FREE_DIFF
    };
private:
    QDateTime time;
    quint64 address;
    quint64 size;
    quint64 new_address;
    diff_type_e type;
public:
    ActiveSessionMemoryBlockDiff(QObject *parent, diff_type_e type, QDateTime time, quint64 address, quint64 size, quint64 new_address)
        : QObject(parent), type(type), time(time), address(address), size(size), new_address(new_address) {}
    virtual ~ActiveSessionMemoryBlockDiff() {}
    
    QDateTime &get_time() { return time; }
    quint64 get_address() const { return address; }
    quint64 get_size() const { return size; }
    quint64 get_new_address() const { return new_address; }
    diff_type_e get_type() const { return type; }
};

class ActiveSessionMemoryBlock : public QObject { Q_OBJECT
private:
    quint64 address;
    quint64 size;
public:
    ActiveSessionMemoryBlock(QObject* parent, quint64 address, quint64 size)
        : QObject(parent), address(address), size(size) {}
    virtual ~ActiveSessionMemoryBlock() {}
    
    quint64 get_address() const { return address; }
    quint64 get_size() const { return size; }
    void set_size(quint64 new_size) { size = new_size; }
};

class ActiveSessionMemory : public QObject { Q_OBJECT
private:
    QByteArray unprocessed;
    quint64 pop_uint64();
public:
    ActiveSessionMemory(QObject *parent = NULL);
    virtual ~ActiveSessionMemory() {}
public slots:
    void process_data(QByteArray data);
signals:
    void diff_processed(ActiveSessionMemoryBlockDiff *);
};

#endif
