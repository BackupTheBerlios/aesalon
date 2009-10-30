#ifndef AESALON_GUI_FRAMEWORK_H
#define AESALON_GUI_FRAMEWORK_H

#include <QtGui/QWidget>

namespace Aesalon {
namespace GUI {

class Framework : public QWidget { Q_OBJECT
public:
    Framework(QWidget *parent = 0);
    virtual ~Framework();
};

} // namespace GUI
} // namespace Aesalon

#endif
