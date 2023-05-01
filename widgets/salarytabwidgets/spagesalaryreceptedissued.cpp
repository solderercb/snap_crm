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
                                    "    t1.`id`, t1.`Title`, t1.`real_repair_cost`, t1.`in_date`, t2.`summ_recept`, t3.`pay_device_in`       \n"\
                                    "FROM `workshop` AS t1                                                                                    \n"\
                                    "LEFT JOIN salary_repairs AS t2                                                                           \n"\
                                    "    ON t1.`id` = t2.`repair` AND t2.`user` = %3 AND t2.`summ_recept` IS NOT NULL                         \n"\
                                    "LEFT JOIN `users` AS t3                                                                                  \n"\
                                    "    ON t1.`manager` = t3.`id`                                                                            \n"\
                                    "WHERE                                                                                                    \n"\
                                    "    IF(%4, 1, t2.`id` IS NULL)                                                                           \n"\
                                    "    AND `in_date` >= '%1'                                                                                \n"\
                                    "    AND `in_date` <= '%2'                                                                                \n"\
                                    "    AND `manager` = %3                                                                                     "\
                                    )                                                                                                            \
                                    .arg(parentTab->periodBegin())                                                                               \
                                    .arg(parentTab->periodEnd())                                                                                 \
                                    .arg(parentTab->m_employeeId)                                                                                \
                                    .arg(ui->checkBoxShowPayed->isChecked())                                                                     \
                                    , QSqlDatabase::database("connMain"));
    ui->tableViewRecepted->refresh();
    parentTab->setModelUpdatedFlag(ReceptedModel);

    ui->tableViewIssued->setQuery(QString(                                                                                                      \
                                  "SELECT                                                                                                    \n"\
                                  "    t2.`id`, t2.`Title`, t2.`real_repair_cost`, t2.`out_date`, t3.`summ_issue`, t4.`pay_device_out`       \n"\
                                  "FROM `workshop_issued` AS `t1`                                                                            \n"\
                                  "INNER JOIN `workshop` AS `t2`                                                                             \n"\
                                  "    ON `t1`.`repair_id` = `t2`.`id`                                                                       \n"\
                                  "LEFT JOIN salary_repairs AS t3                                                                            \n"\
                                  "    ON t1.`repair_id` = t3.`repair` AND t3.`user` = %3 AND t3.`summ_issue` IS NOT NULL                    \n"\
                                  "LEFT JOIN `users` AS t4                                                                                   \n"\
                                  "    ON t1.`employee_id` = t4.`id`                                                                         \n"\
                                  "WHERE                                                                                                     \n"\
                                  "    IF(%4, 1, t3.`id` IS NULL)                                                                            \n"\
                                  "    AND t1.`created_at` >= '%1'                                                                           \n"\
                                  "    AND t1.`created_at` <= '%2'                                                                           \n"\
                                  "    AND t1.`employee_id` = %3                                                                             \n"\
                                  )                                                                                                             \
                                  .arg(parentTab->periodBegin())                                                                                \
                                  .arg(parentTab->periodEnd())                                                                                  \
                                  .arg(parentTab->m_employeeId)                                                                                 \
                                  .arg(ui->checkBoxShowPayed->isChecked())                                                                      \
                                  , QSqlDatabase::database("connMain"));
    ui->tableViewIssued->refresh();
    parentTab->setModelUpdatedFlag(IssuedModel);

    updateWidgets();
}

void SPageSalaryReceptedIssued::updateWidgets()
{
    int recepted = parentTab->m_recepted->rowCount();
    int issued = parentTab->m_issued->rowCount();
    double summReceptedIssued = parentTab->m_recepted->total(STableSalaryReceptedIssued::IncludePayed) + parentTab->m_issued->total(STableSalaryReceptedIssued::IncludePayed);
    double summToPayReceptedIssued = parentTab->m_recepted->total() + parentTab->m_issued->total();
    ui->labelPayForReceptValue->setText(QString::number(parentTab->m_userModel->payDeviceIn()));
    ui->labelPayForIssueValue->setText(QString::number(parentTab->m_userModel->payDeviceOut()));
    ui->labelReceptedSummaryValue->setText(QString::number(recepted));
    ui->labelIssuedSummaryValue->setText(QString::number(issued));
    ui->labelRepairsSummaryValue->setText(QString::number(recepted + issued));
    ui->labelSummValue->setText(sysLocale.toString(summReceptedIssued, 'f', comSettings->value("classic_kassa").toBool()?2:0));
    ui->labelSummToPayValue->setText(sysLocale.toString(summToPayReceptedIssued, 'f', comSettings->value("classic_kassa").toBool()?2:0));
}

void SPageSalaryReceptedIssued::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(index.model()->index(index.row(), 0).data().toInt());
}
