#include "spagesalaryrepairs.h"
#include "ui_spagesalaryrepairs.h"

SPageSalaryRepairs::SPageSalaryRepairs(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryRepairs)
{
    ui->setupUi(this);

    ui->tableViewRepairsSummary->setGridLayout(ui->tableViewRepairs->gridLayout());
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::signalColumnResized, this, &SPageSalaryRepairs::repairsTableColumnResized);

    ui->tableViewRepairs->setModel(parentTab->m_repairs);
    ui->tableViewRepairWorks->setModel(parentTab->m_repairWorks);
    ui->tableViewRepairParts->setModel(parentTab->m_repairParts);

    connect(ui->tableViewRepairs->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SPageSalaryRepairs::tableRepairsRowSelected);
}

SPageSalaryRepairs::~SPageSalaryRepairs()
{
    delete ui;
}

void SPageSalaryRepairs::loadRepairData(const int id)
{
    ui->tableViewRepairWorks->setQuery(QString(
                                       "SELECT `price_id`, `name`,`count`,`price`,`count`*`price` AS 'summ',`warranty` FROM works WHERE `repair` = %1 AND user = %2;"
                                       )
                                       .arg(id)\
                                       .arg(m_userModel->id())\
                                       , QSqlDatabase::database("connMain"));
    ui->tableViewRepairWorks->refresh();

    ui->tableViewRepairParts->setQuery(QString(
                                       "SELECT CONCAT(t2.`articul`, '-', t1.`item_id`) AS 'UID', t1.`name`, t1.`count`, t1.`price`, t1.`count` * t1.`price` AS 'summ', t1.`sn`, t1.`warranty` FROM store_int_reserve AS t1 LEFT JOIN store_items AS t2 ON t1.`item_id` = t2.`id` WHERE `repair_id` = %1 AND `to_user` = %2;"
                                       )
                                       .arg(id)\
                                       .arg(m_userModel->id())\
                                       , QSqlDatabase::database("connMain"));
    ui->tableViewRepairParts->refresh();

}

void SPageSalaryRepairs::updateModels()
{
    QStringList query_group;
    query_group << "`id`";    // default GROUP part of query

    ui->tableViewRepairs->setQuery(QString(                                                                                                                                                                \
                                   "SELECT                                                                                                                                                                \n"\
                                   "    `id`,                                                                                                                                                             \n"\
                                   "    `Title`,                                                                                                                                                          \n"\
                                   "    `real_repair_cost`,                                                                                                                                               \n"\
                                   "    SUM(`work_by_employee`) AS 'works_by_employee',                                                                                                                   \n"\
                                   "    `parts_cost`,                                                                                                                                                     \n"\
                                   "    SUM(`employee_parts`) AS 'employee_parts',                                                                                                                        \n"\
                                   "    SUM(`employee_salary_for_work`) AS 'employee_salary_for_works',                                                                                                                                          \n"\
                                   "    SUM(`employee_salary_for_parts`) AS 'employee_salary_for_parts',                                                                    \n"\
                                   "    `out_date`,                                                                                                                                                     \n"\
                                   "    `state`,                                                                                                                                                          \n"\
                                   "    `status_id`,                                                                                                                                                      \n"\
                                   "    `filter1`                                                                                                                                                         \n"\
                                   "FROM (                                                                                                                                                                \n"\
                                   "    SELECT                                                                                                                                                            \n"\
                                   "        g1t1.*,                                                                                                                                                       \n"\
                                   "        g1t2.`created_at`,                                                                                                                                            \n"\
                                   "        (g1t3.`price`*g1t3.`count`) AS 'work_by_employee',                                                                                                            \n"\
                                   "        (g1t3.`price`*g1t3.`count`*g1t3.`pay_repair`/100) AS 'employee_salary_for_work',                                                                              \n"\
                                   "        IFNULL(SUM(g1t4.`price`*g1t4.`count`), 0) AS 'employee_parts',                                                                                                \n"\
                                   "        IF(g1t5.`pay_4_sale_in_repair`, IFNULL(SUM((g1t4.`price` - g1t6.`in_price`)*g1t4.`count`*IF(g1t1.quick_repair, g1t5.`pay_repair_q_sale`, g1t5.`pay_sale`)/100), 0), 0) AS 'employee_salary_for_parts',\n"\
                                   "        g1t2.`status_id`,                                                                                                                                             \n"\
                                   "        g1t2.`filter1`                                                                                                                                                \n"\
                                   "    FROM workshop AS g1t1                                                                                                                                             \n"\
                                   "    LEFT JOIN (                                                                                                                                                       \n"\
                                   "        SELECT                                                                                                                                                        \n"\
                                   "            `repair_id`                                                                                                                                               \n"\
                                   "            , `created_at`                                                                                                                                            \n"\
                                   "            , `status_id`                                                                                                                                             \n"\
                                   "            , IF(@id<>`repair_id`, @cnt:=1, @cnt:=@cnt+1) AS `filter1`                                                                                                \n"\
                                   "            , @id:=`repair_id`                                                                                                                                        \n"\
                                   "        FROM repair_status_logs CROSS JOIN (SELECT @cnt:=1, @id:=0) AS dummy                                                                                          \n"\
                                   "        ORDER BY `repair_id` ASC, `id` DESC                                                                                                                           \n"\
                                   "    ) AS g1t2                                                                                                                                                         \n"\
                                   "        ON g1t1.`id` = g1t2.`repair_id`                                                                                                                               \n"\
                                   "    LEFT JOIN works AS g1t3                                                                                                                                           \n"\
                                   "        ON g1t1.`id` = g1t3.`repair`                                                                                                                                  \n"\
                                   "    LEFT JOIN store_int_reserve AS g1t4                                                                                                                               \n"\
                                   "        ON g1t3.`id` = g1t4.`work_id` AND g1t4.`to_user` = %4                                                                                                         \n"\
                                   "    LEFT JOIN users AS g1t5                                                                                                                                           \n"\
                                   "        ON g1t3.`user` = g1t5.`id`                                                                                                                                    \n"\
                                   "    LEFT JOIN store_items AS g1t6                                                                                                                                     \n"\
                                   "        ON g1t4.`item_id` = g1t6.`id`                                                                                                                                 \n"\
                                   "    LEFT JOIN salary_repairs AS g1t7                                                                                                                                  \n"\
                                   "        ON g1t1.`id` = g1t7.`repair` AND g1t7.`user` = %4                                                                                                             \n"\
                                   "    WHERE                                                                                                                                                             \n"\
                                   "        g1t7.`id` IS NULL                                                                                                                                             \n"\
                                   "        AND g1t2.`filter1`  = 1                                                                                                                                       \n"\
                                   "        AND `created_at` > '%1'                                                                                                                                       \n"\
                                   "        AND `created_at` < '%2'                                                                                                                                       \n"\
                                   "        AND g1t1.`state` IN (%3)                                                                                                                                      \n"\
                                   "        AND g1t3.`user` = %4                                                                                                                                          \n"\
                                   "    GROUP BY g1t3.id                                                                                                                                                  \n"\
                                   ") AS t1                                                                                                                                                               \n"\
                                   )\
                                   .arg(parentTab->periodBegin())\
                                   .arg(parentTab->periodEnd())\
                                   .arg((ui->checkBoxIncludeNotIssued->isChecked())?"8,12,16,6,7":"8,12,16")\
                                   .arg(parentTab->m_employeeId)\
                                   , QSqlDatabase::database("connMain"));

    ui->tableViewRepairs->setGrouping(query_group);
    ui->tableViewRepairs->refresh();

    parentTab->setModelUpdatedFlag(1);
}

void SPageSalaryRepairs::updateWidgets()
{
    double notUssuedValue = 0;
    m_userModel = parentTab->m_userModel;
    ui->labelPercentWorksValue->setText(QString::number(m_userModel->payRepair()));
    ui->labelPercentWorksQuickValue->setText(QString::number(m_userModel->payRepairQuick()));
    ui->labelPercentPartsValue->setText(QString::number(m_userModel->pay4SaleInRepair()?m_userModel->paySale():0));
    ui->labelPercentRefillValue->setText(QString::number(m_userModel->payCartridgeRefill()));
//    ui->labelSalaryValue->setText(QString::number(m_userModel->pay...()));
//    ui->labelNotUssuedValue->setText(QString::number(m_userModel->pay...()));

    ui->tableViewRepairsSummary->setTotal(0, parentTab->m_repairs->rowCount());
    for(int i = 2; i < 8; i++)
    {
        ui->tableViewRepairsSummary->setTotal(i, parentTab->m_repairs->total(i));
    }
    if(ui->checkBoxIncludeNotIssued->isChecked())
        notUssuedValue = parentTab->m_repairs->notIssuedTotal(6) + parentTab->m_repairs->notIssuedTotal(7);

    ui->labelSalaryValue->setText(sysLocale.toString(parentTab->m_repairs->total(6) + parentTab->m_repairs->total(7), 'f', comSettings->value("classic_kassa").toBool()?2:0));
    ui->labelNotUssuedValue->setText(sysLocale.toString(notUssuedValue, 'f', comSettings->value("classic_kassa").toBool()?2:0));
}

void SPageSalaryRepairs::repairsModelReset()
{
}

void SPageSalaryRepairs::tableRepairsRowActivated(const QModelIndex &index)
{
    loadRepairData(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalaryRepairs::tableRepairsRowSelected(const QModelIndex &current, const QModelIndex &prev)
{
    tableRepairsRowActivated(current);
}

void SPageSalaryRepairs::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalaryRepairs::repairsTableColumnResized(int, int)
{
    ui->tableViewRepairsSummary->applyGridlayout();
}


STableViewSalaryRepairsSummary::STableViewSalaryRepairsSummary(QWidget *parent) : STableViewBase(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalHeader()->setVisible(false);
    dataModel = new QStandardItemModel();
    setModel(dataModel);
    setBackgroundRole(QPalette::Window);
}

STableViewSalaryRepairsSummary::~STableViewSalaryRepairsSummary()
{
    i_gridLayout = nullptr;
}

void STableViewSalaryRepairsSummary::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
        *emptyModelData << new QStandardItem();
    for(int i = 2; i < 8; i++)
    {
        if(i == 6) continue;
        emptyModelData->at(i)->setText("<value>");
    }
    dataModel->appendRow(*emptyModelData);
    setRowHeight(0, this->height());
}

void STableViewSalaryRepairsSummary::setTotal(int column, double value)
{
    dataModel->setData(dataModel->index(0, column), value);
}

void STableViewSalaryRepairsSummary::applyGridlayout()
{
    STableViewBase::applyGridlayout();
}

