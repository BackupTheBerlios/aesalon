#ifndef AESALON_GUI_PROGRAM_REFERENCE_DISPLAY_H
#define AESALON_GUI_PROGRAM_REFERENCE_DISPLAY_H

#include <QWidget>

namespace Aesalon {
namespace GUI {

class ProgramReferenceDisplay : public QWidget { Q_OBJECT
public:
    ProgramReferenceDisplay(QWidget *parent = 0);
    virtual ~ProgramReferenceDisplay();
};

} // namespace GUI
} // namespace Aesalon

#endif
