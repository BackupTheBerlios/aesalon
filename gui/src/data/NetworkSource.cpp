#include "NetworkSource.h"

void NetworkSource::save_to(QSettings *settings) {
    settings->setValue("name", get_name());
    settings->setValue("type", "network");
    settings->setValue("network-host", host);
    settings->setValue("network-port", port);
}
