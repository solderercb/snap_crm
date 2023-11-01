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
    STableViewSummaryBase(SLocalSettings::SalaryPaymentsGrid, parent)
{
    setBackgroundRole(QPalette::Window);
//    setStyleSheet("background-color: rgb(240, 240, 240);");
}

void STableViewSalaryPaymentsSummary::setGridLayout(XtraSerializer *layout)
{
    setData(0, 4, "<value>");
    setData(0, 5, "<value>");
}
