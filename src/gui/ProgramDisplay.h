#ifndef AESALON_GUI_PROGRAM_DISPLAY_H
#define AESALON_GUI_PROGRAM_DISPLAY_H

#include <QMdiSubWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

#include "ProgramBlockDisplay.h"
#include "ProgramReferenceDisplay.h"

namespace Aesalon {
namespace GUI {

class ProgramDisplay : public QMdiSubWindow { Q_OBJECT
private:
    ProgramBlockDisplay *program_block_display;
    ProgramReferenceDisplay *program_reference_display;
    
    QGridLayout *launch_layout;
    QHBoxLayout *launch_program_layout;
    QLineEdit *launch_program_name;
    QLabel *launch_program_label;
    
    void create_launch_widgets();
private slots:
    void begin_program();
public:
    ProgramDisplay(QWidget *parent = 0);
    virtual ~ProgramDisplay();
};

} // namespace GUI
} // namespace Aesalon

#endif
