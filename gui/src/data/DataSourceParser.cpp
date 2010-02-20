#include "session/DataThread.h"
#include "DataSourceParser.h"
#include "NetworkSource.h"

DataSource *DataSourceParser::parse_data_source(QSettings *settings) {
    QString name = settings->value("name").toString();
    QString type = settings->value("type").toString();
    if(type == "network") {
        QString host = settings->value("network-host").toString();
        quint16 port = settings->value("network-port").toUInt();
        return new NetworkSource(name, host, port);
    }
    else return NULL;
}
