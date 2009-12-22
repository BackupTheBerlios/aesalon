#include <iostream>
#include <QSettings>
#include "ProgramDisplay.h"
#include "ProgramDisplay.moc"

#include "misc/String.h"

namespace Aesalon {
namespace GUI {

ProgramDisplay::ProgramDisplay(QWidget *parent) {
    this->hide();
    this->resize(600, 300);
    this->setWindowTitle("Unlaunched program");
    
    create_launch_widget();
}

ProgramDisplay::~ProgramDisplay() {
    
}

void ProgramDisplay::create_launch_widget() {
    QSettings settings;
    launch_widget = new QWidget();
    launch_layout = new QVBoxLayout();
    
    launch_program_layout = new QFormLayout();
    
    launch_program_name = new QComboBox();
    launch_program_name->setEditable(true);
    launch_program_name->setInsertPolicy(QComboBox::InsertAtTop);
    launch_program_name->insertItems(0, settings.value("Program/executable", "").toStringList());
    launch_program_name->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    launch_program_layout->addRow(tr("Executable path:"), launch_program_name);
    
    launch_program_arguments = new QComboBox();
    launch_program_arguments->setEditable(true);
    launch_program_arguments->setInsertPolicy(QComboBox::InsertAtTop);
    launch_program_arguments->addItems(settings.value("Program/arguments", "").toStringList());
    launch_program_arguments->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    launch_program_layout->addRow(tr("Program arguments:"), launch_program_arguments);
    
    launch_port = new QSpinBox();
    launch_port->setMinimum(1024);
    launch_port->setMaximum(65535);
    launch_port->setValue(settings.value("Program/port", 6321).toInt());
    launch_port->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    launch_program_layout->addRow(tr("TCP port to use: "), launch_port);
    
    launch_program_xterm = new QCheckBox("&Launch executable in terminal");
    launch_program_layout->addRow(launch_program_xterm);
    
    launch_program_layout->setFormAlignment(Qt::AlignTop | Qt::AlignRight);
    launch_program_layout->setLabelAlignment(Qt::AlignLeft);
    launch_program_layout->setHorizontalSpacing(25);
    launch_layout->addLayout(launch_program_layout);
    
    launch_layout->addStretch();
    launch_program_button = new QPushButton(tr("&Begin execution"));
    launch_program_button->show();
    connect(launch_program_button, SIGNAL(clicked()), this, SLOT(begin_program()));
    launch_layout->addWidget(launch_program_button, 1, Qt::AlignBottom);
    
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
    running_general_program_name_label = new QLabel(launch_program_name->lineEdit()->text());
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
    
    setWindowTitle(tr("Running program (") + launch_program_name->currentText() + tr(")"));
}

void ProgramDisplay::begin_program() {
    create_running_widget();
    
    program = new Program();
}

bool ProgramDisplay::close() {
    if(!program->is_running()) return QMdiSubWindow::close();
    return false;
}

} // namespace GUI
} // namespace Aesalon
