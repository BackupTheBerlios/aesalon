/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file main/Configuration.h
*/

#ifndef AESALON_GUI_CONFIGURATION_H
#define AESALON_GUI_CONFIGURATION_H

#include <QWidget>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPushButton>
#include <QSpinBox>

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
    QSpinBox *default_snapshot_interval;
    QSpinBox *default_full_snapshot_interval;
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

#endif
