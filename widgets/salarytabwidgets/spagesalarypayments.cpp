#include "spagesalarypayments.h"
#include "ui_spagesalarypayments.h"

SPageSalaryPayments::SPageSalaryPayments(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryPayments)
{
    ui->setupUi(this);
    ui->tableViewPaymentsSummary->setGridLayout(ui->tableViewPayments->gridLayout());
    connect(ui->tableViewPayments, &STableViewSalaryPayments::signalColumnResized, this, &SPageSalaryPayments::repairsTableColumnResized);
    ui->tableViewPayments->setModel(parentTab->m_payments);
}

SPageSalaryPayments::~SPageSalaryPayments()
{
    delete ui;
}

void SPageSalaryPayments::updateModels()
{
    ui->tableViewPayments->setQuery(QString(
                                        "SELECT                                              \n"\
                                        "    t1.`id`,                                        \n"\
                                        "    t1.`payment_date`,                              \n"\
                                        "    t1.`summ`,                                      \n"\
                                        "    t2.`username` AS 'employee',                    \n"\
                                        "    t3.`username` AS 'issuer',                      \n"\
                                        "    t1.`notes`                                      \n"\
                                        "FROM `salary` AS t1                                 \n"\
                                        "INNER JOIN `users` AS t2                            \n"\
                                        "    ON t1.`user_id` = t2.`id`                       \n"\
                                        "INNER JOIN `users` AS t3                            \n"\
                                        "    ON t1.`from_user` = t3.`id`                     \n"\
                                        "WHERE                                               \n"\
                                        "    t1.`period_from` >= '%1'                        \n"\
                                        "    AND t1.`period_to` <= '%2'                      \n"\
                                        "    AND t1.`user_id` = %3;                          \n"\
                                        )                                                       \
                                        .arg(parentTab->periodBegin())                          \
                                        .arg(parentTab->m_periodEnd.addSecs(-1).toString("yyyy-MM-dd hh:mm:ss"))    \
                                        .arg(parentTab->m_employeeId)
                                    , QSqlDatabase::database("connMain"));

    ui->tableViewPayments->refresh();
    parentTab->setModelUpdatedFlag(PaymentsModel);
}

void SPageSalaryPayments::updateWidgets()
{
    ui->tableViewPaymentsSummary->setTotal(2, parentTab->m_payments->total(2));

}

void SPageSalaryPayments::repairsTableColumnResized(int, int)
{
    ui->tableViewPaymentsSummary->applyGridlayout();
}

STableViewSalaryPaymentsSummary::STableViewSalaryPaymentsSummary(QWidget *parent) : STableViewBase(parent)
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
    emptyModelData->at(2)->setText("<value>");
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

