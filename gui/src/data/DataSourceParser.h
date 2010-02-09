#ifndef AESALON_GUI_DATA_SOURCE_PARSER_H
#define AESALON_GUI_DATA_SOURCE_PARSER_H

#include <QSettings>

#include "DataSource.h"

class DataSourceParser {
public:
    static DataSource *parse_data_source(QSettings *settings);
};

#endif
