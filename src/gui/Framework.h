#ifndef AESALON_GUI_FRAMEWORK_H
#define AESALON_GUI_FRAMEWORK_H

#include <QtGui/QWidget>
#include <QtGui/QGridLayout>

namespace Aesalon {
namespace GUI {

class Framework : public QWidget { Q_OBJECT
private:
    QGridLayout *main_layout;
public:
    Framework(QWidget *parent = 0);
    virtual ~Framework();
};

} // namespace GUI
} // namespace Aesalon

#endif
