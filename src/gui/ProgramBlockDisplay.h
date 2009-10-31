#ifndef AESALON_GUI_PROGRAM_BLOCK_DISPLAY_H
#define AESALON_GUI_PROGRAM_BLOCK_DISPLAY_H

#include <QMdiSubWindow>

namespace Aesalon {
namespace GUI {

class ProgramBlockDisplay : public QMdiSubWindow {
public:
    ProgramBlockDisplay(QWidget *parent = 0);
    virtual ~ProgramBlockDisplay();
};

} // namespace GUI
} // namespace Aesalon

#endif
