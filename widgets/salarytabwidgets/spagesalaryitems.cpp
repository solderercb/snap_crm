#include "spagesalaryitems.h"
#include "ui_spagesalaryitems.h"

SPageSalaryItems::SPageSalaryItems(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryItems)
{
    ui->setupUi(this);
    SPageSalaryItems::guiFontChanged();
    ui->tableViewItemsSummary->setGridLayout(ui->tableViewItems->gridLayout());
    connect(ui->tableViewItems, &STableViewSalaryItems::signalColumnResized, this, &SPageSalaryItems::tableColumnResized);

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
    ui->tableViewItems->setQuery(QUERY_SEL_SALARY_PARTS(parentTab->employeeId()));

        ui->tableViewItems->refresh();

        parentTab->setModelUpdatedFlag(ItemsModel);

}

void SPageSalaryItems::updateWidgets()
{
    int totalItemsPeaces = parentTab->m_items->total(STableSalaryItemsModel::Columns::Qty);
    double totalItemsCost = parentTab->m_items->total(STableSalaryItemsModel::Columns::Amount);

    ui->labelTotalItemsByArticulValue->setText(QString::number(parentTab->m_items->rowCount()));
    ui->labelTotalItemsPeacesValue->setText(QString::number(totalItemsPeaces));
    ui->labelTotalItemsCostValue->setText(sysLocale.toString(totalItemsCost, 'f', comSettings->classicKassa?2:0));

    ui->tableViewItemsSummary->setTotal(STableViewSalaryItems::Column::UID, parentTab->m_items->rowCount());
    ui->tableViewItemsSummary->setTotal(STableViewSalaryItems::Column::Qty, totalItemsPeaces);
    ui->tableViewItemsSummary->setTotal(STableViewSalaryItems::Column::Amount, totalItemsCost);

    ui->labelUsedItemsByArticulValue->setText(QString::number(parentTab->m_items->totalUsedItemsByArticul()));
    ui->labelUsedItemsPeacesValue->setText(QString::number(parentTab->m_items->totalUsedItemsPeaces()));
    ui->labelUsedItemsCostValue->setText(sysLocale.toString(parentTab->m_items->totalUsedItemsCost(), 'f', comSettings->classicKassa?2:0));

    ui->labelUnusedItemsByArticulValue->setText(QString::number(parentTab->m_items->totalUnusedItemsByArticul()));
    ui->labelUnusedItemsPeacesValue->setText(QString::number(parentTab->m_items->totalUnusedItemsPeaces()));
    ui->labelUnusedItemsCostValue->setText(sysLocale.toString(parentTab->m_items->totalUnusedItemsCost(), 'f', comSettings->classicKassa?2:0));
}

void SPageSalaryItems::tableItemsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSparePart(parentTab->m_items->index(index.row(), STableSalaryItemsModel::Columns::ItemId).data().toInt());
}

void SPageSalaryItems::tableColumnResized(int, int)
{
    enableVScrollbarPadding(ui->tableViewItems->verticalScrollBar()->isVisible());
    ui->tableViewItemsSummary->applyGridlayout();
}

void SPageSalaryItems::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);
    font.setBold(true);

    ui->labelTotalItemsByArticulValue->setFont(font);
    ui->labelTotalItemsCostValue->setFont(font);
    ui->labelTotalItemsPeacesValue->setFont(font);
    ui->labelUnusedItemsByArticulValue->setFont(font);
    ui->labelUnusedItemsCostValue->setFont(font);
    ui->labelUnusedItemsPeacesValue->setFont(font);
    ui->labelUsedItemsByArticulValue->setFont(font);
    ui->labelUsedItemsCostValue->setFont(font);
    ui->labelUsedItemsPeacesValue->setFont(font);
}

STableViewSalaryItemsSummary::STableViewSalaryItemsSummary(QWidget *parent) :
    STableViewSummaryBase(SLocalSettings::SalaryItemsGrid, parent)
{
    setBackgroundRole(QPalette::Window);
//    setStyleSheet("background-color: rgb(240, 240, 240);");
}

void STableViewSalaryItemsSummary::setGridLayout(XtraSerializer *layout)
{
    STableViewSummaryBase::setGridLayout(layout);
    setData(0, STableViewSalaryItems::Column::Qty, "<value>");
    setData(0, STableViewSalaryItems::Column::Amount, "<value>");
}
