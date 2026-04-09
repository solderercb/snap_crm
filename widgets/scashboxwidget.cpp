#include "scashboxwidget.h"
#include "ui_scashboxwidget.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <QScrollBar>
#include <SUserSettings>
#include <SPaymentTypesModel>
#include <MainWindow>
#include <STableCashboxModel>
#include <STableViewCashbox>

SCashboxWidget::SCashboxWidget(QWidget *parent) :
    SWidget(parent),
    ui(new Ui::SCashboxWidget)
{
    ui->setupUi(this);

    m_mainTableModel = new STableCashboxModel(this);
    ui->tableView->setModel(m_mainTableModel);
    ui->tableView->setQuery(QUERY_SEL_CASHBOX_STATIC, QSqlDatabase::database(TdConn::main()));
    ui->tableView->setUniqueIdColumn(0);
    ui->tableView->enableAutorefresh(userDbData->refreshTime()*1000);

    ui->tableViewSummary->setGridLayout(ui->tableView->gridLayout());
    connect(ui->tableView, &STableViewCashbox::signalColumnResized, this, &SCashboxWidget::tableColumnResized);
    connect(m_mainTableModel, &STableCashboxModel::modelReset, this, &SCashboxWidget::updateSummary);
    connect(ui->tableView, &STableViewCashbox::doubleClicked, this, &SCashboxWidget::tableRowDoubleClicked);
}

SCashboxWidget::~SCashboxWidget()
{
    delete ui;
}

void SCashboxWidget::setFilter(const FilterList &filter)
{
    ui->tableView->setFilter(filter);
}

void SCashboxWidget::refresh(bool preserveScrollPos, bool preserveSelection)
{
    ui->tableView->refresh(preserveScrollPos, preserveSelection);
}

void SCashboxWidget::delayedRefresh(const int msec)
{
    ui->tableView->delayedRefresh(msec);
}

STableCashboxModel *SCashboxWidget::tableModel()
{
    return m_mainTableModel;
}

void SCashboxWidget::enableVScrollbarPadding(const bool state)
{
    ui->gridLayout->setColumnMinimumWidth(1, state*ui->tableView->verticalScrollBar()->width());
}

void SCashboxWidget::updateSummary()
{
    for(int i = STableViewCashbox::Column::AmountsByCash; i < STableViewCashbox::Column::PaymentType; i++)
    {
        double total = 0;
        double rowTotal = 0;

        rowTotal = m_mainTableModel->total(i, SPaymentTypesModel::PKO);
        total += rowTotal;
        ui->tableViewSummary->setTotal(0, i, rowTotal);

        rowTotal = m_mainTableModel->total(i, SPaymentTypesModel::RKO);
        total += rowTotal;
        ui->tableViewSummary->setTotal(1, i, rowTotal);

        ui->tableViewSummary->setTotal(2, i, total);
    }
}

void SCashboxWidget::tableRowDoubleClicked(const QModelIndex &index)
{
    MainWindow::getInstance()->createTabCashOperation(index.siblingAtColumn(STableCashboxModel::Columns::Id).data().toInt());
}

void SCashboxWidget::tableColumnResized(int, int)
{
    enableVScrollbarPadding(ui->tableView->verticalScrollBar()->isVisible());
    ui->tableViewSummary->applyGridlayout();
}
