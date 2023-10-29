#include "spagesalarypayments.h"
#include "ui_spagesalarypayments.h"

SPageSalaryPayments::SPageSalaryPayments(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryPayments)
{
    ui->setupUi(this);
    ui->tableViewPaymentsSummary->setGridLayout(ui->tableViewPayments->gridLayout());
    connect(ui->tableViewPayments, &STableViewSalaryPayments::signalColumnResized, this, &SPageSalaryPayments::tableColumnResized);
    ui->tableViewPayments->setModel(parentTab->m_payments);
}

SPageSalaryPayments::~SPageSalaryPayments()
{
    delete ui;
}

void SPageSalaryPayments::updateModels()
{
    ui->tableViewPayments->setQuery(QUERY_SEL_SALARY_PAYMENTS(
                                        parentTab->periodBegin(),
                                        parentTab->m_periodEnd.addSecs(-1).toString("yyyy-MM-dd hh:mm:ss"),
                                        parentTab->employeeId()));

    ui->tableViewPayments->refresh();
    parentTab->setModelUpdatedFlag(PaymentsModel);
}

void SPageSalaryPayments::updateWidgets()
{
    ui->tableViewPaymentsSummary->setTotal(4, parentTab->m_payments->STableBaseModel::total(4));
    ui->tableViewPaymentsSummary->setTotal(5, parentTab->m_payments->STableBaseModel::total(5));
}

void SPageSalaryPayments::tableColumnResized(int, int)
{
    ui->tableViewPaymentsSummary->applyGridlayout();
}

STableViewSalaryPaymentsSummary::STableViewSalaryPaymentsSummary(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryPaymentsGrid, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalHeader()->setVisible(false);
    dataModel = new QStandardItemModel();
    setModel(dataModel);
    setBackgroundRole(QPalette::Window);
}

STableViewSalaryPaymentsSummary::~STableViewSalaryPaymentsSummary()
{
    i_gridLayout = nullptr;
}

void STableViewSalaryPaymentsSummary::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
        *emptyModelData << new QStandardItem();
    emptyModelData->at(4)->setText("<value>");
    emptyModelData->at(5)->setText("<value>");
    dataModel->appendRow(*emptyModelData);
    setRowHeight(0, this->height());
}

void STableViewSalaryPaymentsSummary::setTotal(int column, double value)
{
    dataModel->setData(dataModel->index(0, column), value);
}

void STableViewSalaryPaymentsSummary::applyGridlayout()
{
    STableViewBase::applyGridlayout();
}

