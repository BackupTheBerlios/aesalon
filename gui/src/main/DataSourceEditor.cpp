#include "DataSourceEditor.h"
#include "DataSourceEditor.moc"

#include "data/NetworkSource.h"

DataSourceEditor::DataSourceEditor(QWidget *parent)
    : QDialog(parent), data_source(NULL) {
    
    this->setMinimumSize(400, 50);
    
    main_layout = new QFormLayout();
    
    source_name = new QLineEdit();
    main_layout->addRow(tr("Data source name:"), source_name);
    
    tab_bar = new QTabWidget();
    tab_bar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    main_layout->addRow(tab_bar);
    
    network_tab = new QWidget();
    network_tab->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    network_layout = new QFormLayout();
    network_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    network_host = new QLineEdit();
    network_host->setAlignment(Qt::AlignRight);
    network_layout->addRow(tr("Hostname:"), network_host);
    
    network_port = new QSpinBox();
    network_port->setRange(1025, 65534);
    network_port->setAlignment(Qt::AlignRight);
    network_layout->addRow(tr("Port:"), network_port);
    
    network_tab->setLayout(network_layout);
    tab_bar->addTab(network_tab, tr("&Network connection"));
    
    buttons = new QDialogButtonBox();
    buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    
    connect(buttons, SIGNAL(accepted()), SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), SLOT(reject()));
    main_layout->addRow(buttons);
    
    setLayout(main_layout);
}

DataSourceEditor::~DataSourceEditor() {

}

DataSource *DataSourceEditor::create_new() {
    data_source = NULL;
    source_name->setText("");
    network_host->setText("localhost");
    network_port->setValue(DEFAULT_PORT);
    this->exec();
    return data_source;
}

void DataSourceEditor::edit(DataSource *data_source) {
    this->data_source = data_source;
    source_name->setText(data_source->get_name());
    source_name->setText(tr("Editing NYI!"));
    this->exec();
}

void DataSourceEditor::accept() {
    if(data_source != NULL) {
        data_source->set_name(source_name->text());
    }
    else if(data_source == NULL) {
        if(tab_bar->currentWidget() == network_tab) {
            data_source = new NetworkSource(source_name->text(), network_host->text(), network_port->value());
        }
    }
    
    QDialog::accept();
}
