#ifndef AESALON_GUI_PROGRAM_DISPLAY_H
#define AESALON_GUI_PROGRAM_DISPLAY_H

#include <QMdiSubWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>

#include "ProgramBlockDisplay.h"
#include "ProgramReferenceDisplay.h"

#include "misc/SmartPointer.h"
#include "platform/Program.h"

namespace Aesalon {
namespace GUI {

class ProgramDisplay : public QMdiSubWindow { Q_OBJECT
private:
    ProgramBlockDisplay *program_block_display;
    ProgramReferenceDisplay *program_reference_display;
    
    QWidget *launch_widget;
    QVBoxLayout *launch_layout;
    QGridLayout *launch_program_layout;
    QLineEdit *launch_program_name;
    QLabel *launch_program_label;
    QLineEdit *launch_program_arguments;
    QLabel *launch_program_arguments_label;
    QPushButton *launch_program_button;
    
    QWidget *running_widget;
    QVBoxLayout *running_layout;
    QTabWidget *running_tab_bar;
    QWidget *running_general;
    QVBoxLayout *running_general_layout;
    QHBoxLayout *running_general_program_layout;
    QLabel *running_general_program_label;
    QLabel *running_general_program_name_label;
    
    Platform::Program *program;
    
    void create_launch_widget();
    void create_running_widget();
private slots:
    void begin_program();
public:
    ProgramDisplay(QWidget *parent = 0);
    virtual ~ProgramDisplay();
};

} // namespace GUI
} // namespace Aesalon

#endif
