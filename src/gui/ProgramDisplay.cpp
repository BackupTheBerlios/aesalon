#include "ProgramDisplay.h"
#include "ProgramDisplay.moc"

namespace Aesalon {
namespace GUI {

ProgramDisplay::ProgramDisplay(QWidget *parent) {
    this->setGeometry(0, 0, 300, 200);
    this->setWindowTitle("Unlaunched program");
    
    create_launch_widgets();
}

ProgramDisplay::~ProgramDisplay() {
    
}

void ProgramDisplay::create_launch_widgets() {
    launch_layout = new QGridLayout();
    
    launch_program_layout = new QHBoxLayout();
    
    launch_program_label = new QLabel(tr("Executable path"));
    launch_program_layout->addWidget(launch_program_label);
    launch_program_name = new QLineEdit("");
    launch_program_layout->addWidget(launch_program_name);
    
    launch_layout->addLayout(launch_program_layout, 0, 0);
    
    /*setLayout(launch_layout);*/
}

void ProgramDisplay::begin_program() {
    
}

} // namespace GUI
} // namespace Aesalon
