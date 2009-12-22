#ifndef AESALON_GUI_AESALON_CONFIGURATION_H
#define AESALON_GUI_AESALON_CONFIGURATION_H

#include <QWidget>
#include <QDialog>

namespace Aesalon {
namespace GUI {

class AesalonConfiguration : public QDialog { Q_OBJECT
public:
    AesalonConfiguration(QWidget *parent = 0);
    virtual ~AesalonConfiguration();
};

} // namespace GUI
} // namespace Aesalon

#endif
