#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QMenu>

#include "FilterManager.h"
#include "FilterManager.moc"

FilterManager::TreeItem::TreeItem() : QTreeWidgetItem() {

}

FilterManager::TreeItem::~TreeItem() {

}


FilterManager::FilterManager() {
    QVBoxLayout *main_layout = new QVBoxLayout();
    
    editor = new FilterEditor();
    
    filter_tree = new QTreeWidget();
    
    filter_tree->setHeaderLabel(tr("Type"));
    filter_tree->setSelectionBehavior(QAbstractItemView::SelectItems);
    
    TreeItem *item = new TreeItem();
    item->setText(0, "Operator (All)");
    item->set_filter(root_filter);
    
    filter_tree->addTopLevelItem(item);
    
    main_layout->addWidget(filter_tree);
    
    QMenu *context_menu = new QMenu();
    QAction *action = new QAction(tr("&Add filter"), context_menu);
    action->setShortcut(Qt::Key_Insert);
    connect(action, SIGNAL(triggered(bool)), SLOT(add_filter()));
    
    action = new QAction(tr("&Remove filter"), context_menu);
    action->setShortcut(Qt::Key_Delete);
    connect(action, SIGNAL(triggered(bool)), SLOT(remove_filter()));
    
    filter_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(filter_tree, SIGNAL(customContextMenuRequested(QPoint)), context_menu, SLOT(popup(QPoint)));
    
    setLayout(main_layout);
    
    setWindowModality(Qt::WindowModal);
}

FilterManager::~FilterManager() {

}

void FilterManager::add_filter() {
    QList<QTreeWidgetItem *> items = filter_tree->selectedItems();
    if(items.size() != 1) return;
    TreeItem *item = dynamic_cast<TreeItem *>(items[0]);
    Filter *filter = editor->create_filter();
}

void FilterManager::remove_filter() {
    
}
