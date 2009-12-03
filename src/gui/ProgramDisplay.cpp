#include "ProgramDisplay.h"
#include "ProgramDisplay.moc"

namespace Aesalon {
namespace GUI {

ProgramDisplay::ProgramDisplay(QWidget *parent) {
    this->hide();
    this->setGeometry(0, 0, 400, 300);
    this->setWindowTitle("Unlaunched program");
    
    create_launch_widget();
}

ProgramDisplay::~ProgramDisplay() {
    
}

void ProgramDisplay::create_launch_widget() {
    launch_widget = new QWidget();
    launch_layout = new QVBoxLayout();
    
    launch_program_layout = new QGridLayout();
    
    launch_program_label = new QLabel(tr("Executable path:"));
    launch_program_layout->addWidget(launch_program_label, 0, 0);
    launch_program_name = new QLineEdit("");
    launch_program_layout->addWidget(launch_program_name, 0, 1);
    
    launch_layout->addLayout(launch_program_layout);
    
    
    launch_program_arguments_label = new QLabel(tr("Program arguments:"));
    launch_program_layout->addWidget(launch_program_arguments_label, 1, 0);
    launch_program_arguments = new QLineEdit("");
    launch_program_layout->addWidget(launch_program_arguments, 1, 1);
    
    launch_program_xterm = new QCheckBox("&Launch executable in terminal");
    launch_program_layout->addWidget(launch_program_xterm, 2, 0);
    
    launch_layout->addStretch();
    launch_program_button = new QPushButton(tr("&Begin execution"));
    launch_program_button->show();
    connect(launch_program_button, SIGNAL(clicked()), this, SLOT(begin_program()));
    launch_program_layout->addWidget(launch_program_button, 3, 0, 1, 2, Qt::AlignBottom);
    
    launch_widget->setLayout(launch_layout);
    setWidget(launch_widget);
}

void ProgramDisplay::create_running_widget() {
    running_widget = new QWidget();
    running_layout = new QVBoxLayout();
    running_tab_bar = new QTabWidget();
    
    program_block_display = new ProgramBlockDisplay();
    program_reference_display = new ProgramReferenceDisplay();
    
    
    running_general = new QWidget();
    running_general_layout = new QVBoxLayout();
    
    
    running_general_program_layout = new QHBoxLayout();
    
    running_general_program_label = new QLabel(tr("Executable path:"));
    running_general_program_layout->addWidget(running_general_program_label);
    running_general_program_name_label = new QLabel(launch_program_name->text());
    running_general_program_layout->addWidget(running_general_program_name_label);
    
    running_general_layout->addLayout(running_general_program_layout);
    
    running_general_layout->addStretch();
    
    running_general->setLayout(running_general_layout);
    
    running_tab_bar->addTab(running_general, "&General information");
    running_tab_bar->addTab(program_block_display, "&Block display");
    running_tab_bar->addTab(program_reference_display, "&Reference display");
    
    running_layout->addWidget(running_tab_bar);
    
    running_widget->setLayout(running_layout);
    setWidget(running_widget);
    
    setWindowTitle(tr("Running program (") + launch_program_name->text() + tr(")"));
}

void ProgramDisplay::begin_program() {
    create_running_widget();
}

} // namespace GUI
} // namespace Aesalon
