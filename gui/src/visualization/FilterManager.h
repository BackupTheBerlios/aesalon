#ifndef AESALON_GUI_VISUALIZATION_FILTER_MANAGER_H
#define AESALON_GUI_VISUALIZATION_FILTER_MANAGER_H

#include <QDialog>
#include <QTreeWidget>
#include <QMap>
#include <QMenu>

#include "Filter.h"
#include "BooleanAndFilter.h"
#include "FilterEditor.h"
#include "FilterFactory.h"

class FilterManager : public QDialog { Q_OBJECT
private:
    QTreeWidget *filter_tree;
    QMenu *context_menu;
    BooleanAndFilter *root_filter;
    
    FilterEditor *editor;
    
    class TreeItem : public QTreeWidgetItem {
    private:
        Filter *filter;
    public:
        TreeItem();
        virtual ~TreeItem();
        
        Filter *get_filter() const { return filter; }
        void set_filter(Filter *new_filter) { filter = new_filter; }
    };
public:
    FilterManager();
    virtual ~FilterManager();
    
    Filter *get_root_filter() const { return root_filter; }
private slots:
    void add_filter();
    void remove_filter();
    
    void popup_context_menu(QPoint point);
};

#endif
