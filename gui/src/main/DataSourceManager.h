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
    
    @file main/DataSourceManager.h
*/

#ifndef AESALON_GUI_MAIN_DATA_SOURCE_MANAGER_H
#define AESALON_GUI_MAIN_DATA_SOURCE_MANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "data/DataSource.h"
#include "session/Session.h"
#include "DataSourceEditor.h"

class DataSourceManager : public QWidget { Q_OBJECT
private:
    QVBoxLayout *main_layout;
    QListWidget *source_list;
    QList<DataSource *> data_source_list;
    QHBoxLayout *lower_button_layout;
    QPushButton *new_source;
    QPushButton *create_session;
    DataSourceEditor *editor;
public:
    DataSourceManager(QWidget *parent);
    virtual ~DataSourceManager();
private slots:
    void load_from_config();
    void save_to_config();
    void update_list();
    void launch_session();
    void create_data_source();
    void edit_data_source(QListWidgetItem *item);
public slots:
    void add_new_data_source(DataSource *data_source);
    void data_source_changed(DataSource *data_source);
signals:
    void session_launched(Session *session);
    void tab_request(QWidget *widget, QString name);
};

#endif
