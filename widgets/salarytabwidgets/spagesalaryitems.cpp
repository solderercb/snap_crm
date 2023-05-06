#include "spagesalaryitems.h"
#include "ui_spagesalaryitems.h"

SPageSalaryItems::SPageSalaryItems(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryItems)
{
    ui->setupUi(this);

    ui->tableViewItemsSummary->setGridLayout(ui->tableViewItems->gridLayout());
    connect(ui->tableViewItems, &STableViewSalaryItems::signalColumnResized, this, &SPageSalaryItems::repairsTableColumnResized);

    ui->tableViewItems->setModel(parentTab->m_items);

    connect(ui->tableViewItems, &STableViewSalaryItems::doubleClicked, this, &SPageSalaryItems::tableItemsRowDoubleClicked);
}

SPageSalaryItems::~SPageSalaryItems()
{
    delete ui;
}

void SPageSalaryItems::enableVScrollbarPadding(const bool state)
{
    ui->gridLayoutItems->setColumnMinimumWidth(1, state*ui->tableViewItems->verticalScrollBar()->width());
}

void SPageSalaryItems::updateModels()
{
    ui->tableViewItems->setQuery(QString(
                                 "SELECT                                                                                       \n"\
                                 "    CONCAT(t2.`articul`, '-', t1.`item_id`) AS 'UID',                                        \n"\
                                 "    t1.`name`,                                                                               \n"\
                                 "    t1.`created`,                                                                            \n"\
                                 "    t1.`count`,                                                                              \n"\
                                 "    t1.`price`,                                                                              \n"\
                                 "    t1.`count` * t1.`price` AS 'summ',                                                       \n"\
                                 "    t1.`state`,                                                                              \n"\
                                 "    t1.`item_id`                                                                             \n"\
                                 "FROM `store_int_reserve` AS t1                                                               \n"\
                                 "LEFT JOIN store_items AS t2                                                                  \n"\
                                 "    ON t1.`item_id` = t2.`id`                                                                \n"\
                                 " WHERE (t1.`to_user` = %1) AND ((1 = t1.`state`) OR (2 = t1.`state`))                          "\
                                 )
                                .arg(parentTab->employeeId())\
                                , QSqlDatabase::database("connMain"));

        ui->tableViewItems->refresh();

        parentTab->setModelUpdatedFlag(ItemsModel);

}

void SPageSalaryItems::updateWidgets()
{
    int totalItemsPeaces = parentTab->m_items->total(3);
    double totalItemsCost = parentTab->m_items->total(5);

    ui->labelTotalItemsByArticulValue->setText(QString::number(parentTab->m_items->rowCount()));
    ui->labelTotalItemsPeacesValue->setText(QString::number(totalItemsPeaces));
    ui->labelTotalItemsCostValue->setText(sysLocale.toString(totalItemsCost, 'f', comSettings->value("classic_kassa").toBool()?2:0));

    ui->tableViewItemsSummary->setTotal(0, parentTab->m_items->rowCount());
    ui->tableViewItemsSummary->setTotal(3, totalItemsPeaces);
    ui->tableViewItemsSummary->setTotal(5, totalItemsCost);

    ui->labelUsedItemsByArticulValue->setText(QString::number(parentTab->m_items->totalUsedItemsByArticul()));
    ui->labelUsedItemsPeacesValue->setText(QString::number(parentTab->m_items->totalUsedItemsPeaces()));
    ui->labelUsedItemsCostValue->setText(sysLocale.toString(parentTab->m_items->totalUsedItemsCost(), 'f', comSettings->value("classic_kassa").toBool()?2:0));

    ui->labelUnusedItemsByArticulValue->setText(QString::number(parentTab->m_items->totalUnusedItemsByArticul()));
    ui->labelUnusedItemsPeacesValue->setText(QString::number(parentTab->m_items->totalUnusedItemsPeaces()));
    ui->labelUnusedItemsCostValue->setText(sysLocale.toString(parentTab->m_items->totalUnusedItemsCost(), 'f', comSettings->value("classic_kassa").toBool()?2:0));
}

void SPageSalaryItems::tableItemsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSparePart(parentTab->m_items->index(index.row(), 7).data().toInt());
}

void SPageSalaryItems::repairsTableColumnResized(int column, int newWidth)
{
    enableVScrollbarPadding(ui->tableViewItems->verticalScrollBar()->isVisible());
    ui->tableViewItemsSummary->applyGridlayout();
}

STableViewSalaryItemsSummary::STableViewSalaryItemsSummary(QWidget *parent) : STableViewBase(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalHeader()->setVisible(false);
    dataModel = new QStandardItemModel();
    setModel(dataModel);
    setBackgroundRole(QPalette::Window);
}

STableViewSalaryItemsSummary::~STableViewSalaryItemsSummary()
{
    i_gridLayout = nullptr;
}

void STableViewSalaryItemsSummary::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
        *emptyModelData << new QStandardItem();
    emptyModelData->at(3)->setText("<value>");
    emptyModelData->at(5)->setText("<value>");
    dataModel->appendRow(*emptyModelData);
    setRowHeight(0, this->height());
}

void STableViewSalaryItemsSummary::setTotal(int column, double value)
{
    dataModel->setData(dataModel->index(0, column), value);
}

void STableViewSalaryItemsSummary::applyGridlayout()
{
    STableViewBase::applyGridlayout();
}

