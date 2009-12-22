#ifndef AESALON_GUI_PROGRAM_DISPLAY_H
#define AESALON_GUI_PROGRAM_DISPLAY_H

#include <QTimer>

#include <QMdiSubWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>

#include "ProgramBlockDisplay.h"
#include "ProgramReferenceDisplay.h"

#include "misc/SmartPointer.h"
#include "Program.h"

namespace Aesalon {
namespace GUI {

class ProgramDisplay : public QMdiSubWindow { Q_OBJECT
private:
    ProgramBlockDisplay *program_block_display;
    ProgramReferenceDisplay *program_reference_display;
    
    QWidget *launch_widget;
    QVBoxLayout *launch_layout;
    QFormLayout *launch_program_layout;
    QComboBox *launch_program_name;
    QComboBox *launch_program_arguments;
    QLabel *launch_port_label;
    QSpinBox *launch_port;
    QCheckBox *launch_program_xterm;
    QPushButton *launch_program_button;
    
    QWidget *running_widget;
    QVBoxLayout *running_layout;
    QTabWidget *running_tab_bar;
    QWidget *running_general;
    QVBoxLayout *running_general_layout;
    QHBoxLayout *running_general_program_layout;
    QLabel *running_general_program_label;
    QLabel *running_general_program_name_label;
    
    QTimer *update_timer;
    
    Misc::SmartPointer<Program> program;
    
    void create_launch_widget();
    void create_running_widget();
public slots:
    bool close();
private slots:
    void begin_program();
public:
    ProgramDisplay(QWidget *parent = 0);
    virtual ~ProgramDisplay();
};

} // namespace GUI
} // namespace Aesalon

#endif
