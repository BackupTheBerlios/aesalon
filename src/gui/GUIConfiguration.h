#ifndef AESALON_GUI_GUICONFIGURATION_H
#define AESALON_GUI_GUICONFIGURATION_H

#include <QDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

namespace Aesalon {
namespace GUI {

class GUIConfiguration : public QDialog { Q_OBJECT
private:
    QVBoxLayout *main_layout;
    
    QTabWidget *tab_widget;
    QWidget *general_tab;
    QFormLayout *general_layout;
    QLineEdit *general_aesalon_path;
    
    QDialogButtonBox *dialog_buttons;
    
    QSettings settings;
public:
    GUIConfiguration(QWidget *parent = 0);
    virtual ~GUIConfiguration();
public slots:
    int exec();
    void accept();
};

} // namespace GUI
} // namespace Aesalon

#endif
