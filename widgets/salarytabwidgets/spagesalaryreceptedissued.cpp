#include "spagesalaryreceptedissued.h"
#include "ui_spagesalaryreceptedissued.h"

SPageSalaryReceptedIssued::SPageSalaryReceptedIssued(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryReceptedIssued)
{
    ui->setupUi(this);

    ui->tableViewRecepted->setModel(parentTab->m_recepted);
    ui->tableViewIssued->setModel(parentTab->m_issued);
}

SPageSalaryReceptedIssued::~SPageSalaryReceptedIssued()
{
    delete ui;
}

void SPageSalaryReceptedIssued::updateModels()
{
    ui->tableViewRecepted->setQuery(QString(                                                                                                     \
                                    "SELECT                                                                                                   \n"\
                                    "    `id`, `Title`, `real_repair_cost`, `in_date`                                                         \n"\
                                    "    FROM `workshop`                                                                                      \n"\
                                    "    WHERE `in_date` >= '%1' AND `in_date` <= '%2' AND `manager` = %3;"                                      \
                                    )                                                                                                            \
                                    .arg(parentTab->periodBegin())                                                                               \
                                    .arg(parentTab->periodEnd())                                                                                 \
                                    .arg(parentTab->m_employeeId)                                                                                \
                                    , QSqlDatabase::database("connMain"));
    ui->tableViewRecepted->refresh();
    parentTab->setModelUpdatedFlag(8);

    ui->tableViewIssued->setQuery(QString(
                                  "SELECT                                                                                                    \n"\
                                  "    t2.`id`, t2.`Title`, t2.`real_repair_cost`, t2.`in_date`                                              \n"\
                                  "    FROM `workshop_issued` AS `t1` INNER JOIN `workshop` AS `t2` ON `t1`.`repair_id` = `t2`.`id`          \n"\
                                  "     WHERE `t1`.`created_at` >= '%1' AND `t1`.`created_at` <= '%2' AND `t1`.`employee_id` = %3;"             \
                                  )                                                                                                             \
                                  .arg(parentTab->periodBegin())                                                                                \
                                  .arg(parentTab->periodEnd())                                                                                  \
                                  .arg(parentTab->m_employeeId)                                                                                 \
                                  , QSqlDatabase::database("connMain"));
    ui->tableViewIssued->refresh();
    parentTab->setModelUpdatedFlag(9);

    updateWidgets();
}

void SPageSalaryReceptedIssued::updateWidgets()
{
    int recepted = parentTab->m_recepted->rowCount();
    int issued = parentTab->m_issued->rowCount();
    ui->labelReceptedSummaryValue->setText(QString::number(recepted));
    ui->labelIssuedSummaryValue->setText(QString::number(issued));
    ui->labelRepairsSummaryValue->setText(QString::number(recepted + issued));
    ui->labelSummValue->setText(sysLocale.toString(parentTab->summRecepedIssued(), 'f', comSettings->value("classic_kassa").toBool()?2:0));
}

void SPageSalaryReceptedIssued::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(index.model()->index(index.row(), 0).data().toInt());
}
