#ifndef AESALON_GUI_CONFIGURATION_H
#define AESALON_GUI_CONFIGURATION_H

#include <QWidget>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPushButton>

namespace Aesalon {
namespace GUI {

class Configuration : public QDialog { Q_OBJECT
private:
    QVBoxLayout *main_layout;
    QFormLayout *form_layout;
    QHBoxLayout *aesalon_path_layout;
    QLineEdit *aesalon_path;
    QPushButton *aesalon_path_select_dialog;
    QFileDialog *aesalon_path_file_selector;    
    QHBoxLayout *xterm_path_layout;
    QLineEdit *xterm_path;
    QPushButton *xterm_path_select_dialog;
    QFileDialog *xterm_path_file_selector;
    QDialogButtonBox *button_box;
    
public:
    Configuration(QWidget *parent = 0);
    virtual ~Configuration() {}    
private slots:
    void change_aesalon_path(QString new_path) {
        aesalon_path->setText(new_path);
    }
    void change_xterm_path(QString new_path) {
        xterm_path->setText(new_path);
    }
public slots:
    void accept();
};

} // namespace GUI
} // namespace Aesalon

#endif
