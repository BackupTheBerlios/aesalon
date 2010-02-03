#include <stdlib.h>

#include <QSettings>

#include "SessionEditor.h"
#include "SessionEditor.moc"

QString SessionEditor::last_directory = getenv("HOME");

SessionEditor::SessionEditor(QWidget *parent, Session *session) : QDialog(parent), session(session) {
    if(this->session == NULL) {
        this->session = new Session();
    }
    create_widgets();
    this->setModal(true);
    QSizePolicy expanding;
    expanding.setHorizontalPolicy(QSizePolicy::Expanding);
    expanding.setVerticalPolicy(QSizePolicy::Expanding);
    this->setSizePolicy(expanding);
    this->setMinimumSize(600, 100);
    this->setWindowTitle(tr("Editing session"));
}

void SessionEditor::create_widgets() {
    QSettings settings;
    layout = new QVBoxLayout();
    
    tab_bar = new QTabWidget();
    layout->addWidget(tab_bar);
    
    basic_form_layout = new QFormLayout();
    basic_form_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    session_name = new QLineEdit(session->get_session_name());
    basic_form_layout->addRow(tr("Session name:"), session_name);
    
    session_type_layout = new QHBoxLayout();
    session_type_group = new QButtonGroup();
    session_type_group->setExclusive(true);
    launch_session_type = new QRadioButton(tr("&Launch local program"));
    connect_session_type = new QRadioButton(tr("&Connect to remote monitor"));
    
    if(session->get_session_type() == Session::LAUNCH_SESSION) launch_session_type->setChecked(true);
    else connect_session_type->setChecked(true);
    
    session_type_group->addButton(launch_session_type);
    session_type_group->addButton(connect_session_type);
    connect(session_type_group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(session_type_changed(QAbstractButton*)));
    
    session_type_layout->addWidget(launch_session_type);
    session_type_layout->addWidget(connect_session_type);
    basic_form_layout->addRow(tr("Session type:"), session_type_layout);
    
    executable_path_layout = new QHBoxLayout();
    executable_path = new QLineEdit(session->get_executable_path());
    executable_path_layout->addWidget(executable_path);
    file_select_dialog = new QPushButton(tr(". . ."));
    connect(file_select_dialog, SIGNAL(pressed()), this, SLOT(show_file_select_dialog()));
    executable_path_layout->addWidget(file_select_dialog);
    executable_path_label = new QLabel(tr("Executable path:"));
    basic_form_layout->addRow(executable_path_label, executable_path_layout);
    
    arguments = new QLineEdit(session->get_arguments());
    arguments_label = new QLabel(tr("Arguments:"));
    basic_form_layout->addRow(arguments_label, arguments);
    
    port = new QSpinBox();
    port->setAlignment(Qt::AlignRight);
    port->setMinimum(1025);
    port->setMaximum(65535);
    if(session->get_port()) port->setValue(session->get_port());
    else port->setValue(settings.value("default-port", DEFAULT_PORT).toInt());
    basic_form_layout->addRow(tr("Port:"), port);
    
    basic_widget = new QWidget();
    basic_widget->setLayout(basic_form_layout);
    tab_bar->addTab(basic_widget, tr("&Basic"));
    
    advanced_form_layout = new QFormLayout();
    advanced_form_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    snapshot_interval = new QSpinBox();
    snapshot_interval->setAlignment(Qt::AlignRight);
    snapshot_interval->setMinimum(50);
    snapshot_interval->setMaximum(60000);
    snapshot_interval->setSuffix(tr("ms"));
    if(session->get_snapshot_interval()) snapshot_interval->setValue(session->get_snapshot_interval());
    else snapshot_interval->setValue(settings.value("default-snapshot-interval", 1000).toInt());
    advanced_form_layout->addRow(tr("Snapshot interval:"), snapshot_interval);
    
    full_snapshot_interval = new QSpinBox();
    full_snapshot_interval->setAlignment(Qt::AlignRight);
    full_snapshot_interval->setMinimum(1);
    full_snapshot_interval->setMaximum(1000);
    full_snapshot_interval->setPrefix(tr("Every "));
    full_snapshot_interval->setSuffix(tr(" snapshot(s)"));
    if(session->get_full_snapshot_interval()) full_snapshot_interval->setValue(session->get_full_snapshot_interval());
    else full_snapshot_interval->setValue(settings.value("default-full-snapshot-interval", 100).toInt());
    advanced_form_layout->addRow(tr("Full snapshot interval:"), full_snapshot_interval);
    
    advanced_widget = new QWidget();
    advanced_widget->setLayout(advanced_form_layout);
    tab_bar->addTab(advanced_widget, tr("&Advanced"));
    
    button_box = new QDialogButtonBox();
    button_box->addButton(QDialogButtonBox::Save);
    button_box->addButton(QDialogButtonBox::Cancel);
    
    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));
    
    button_box->setCenterButtons(false);
    
    layout->addWidget(button_box);
    
    file_dialog = new QFileDialog();
    file_dialog->setDirectory(last_directory);
    connect(file_dialog, SIGNAL(fileSelected(QString)), this, SLOT(change_selected_file(QString)));
    
    this->setLayout(layout);
    
    this->change_session_type(session->get_session_type());
}

void SessionEditor::accept() {
    if(session_name->text().length() == 0) return;
    session->set_session_name(session_name->text());
    session->set_executable_path(executable_path->text());
    session->set_arguments(arguments->text());
    session->set_port(port->value());
    if(launch_session_type->isChecked()) session->set_session_type(Session::LAUNCH_SESSION);
    else session->set_session_type(Session::CONNECT_SESSION);
    session->set_snapshot_interval(snapshot_interval->value());
    session->set_full_snapshot_interval(full_snapshot_interval->value());
    QDialog::accept();
}

void SessionEditor::show_file_select_dialog() {
    file_dialog->exec();
}

void SessionEditor::change_selected_file(QString new_filename) {
    executable_path->setText(new_filename);
    last_directory = new_filename.left(new_filename.lastIndexOf('/'));
    file_dialog->setDirectory(last_directory);
}

void SessionEditor::change_session_type(Session::session_type_e new_type) {
    if(new_type == Session::LAUNCH_SESSION) {
        executable_path_label->setText(tr("Executable path:"));
        file_select_dialog->show();
        arguments->show();
        arguments_label->show();
    }
    else {
        executable_path_label->setText(tr("Host:"));
        file_select_dialog->hide();
        arguments->hide();
        arguments_label->hide();
    }
}
