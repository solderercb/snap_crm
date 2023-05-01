#include "spagesalarysales.h"
#include "ui_spagesalarysales.h"
#include "stableviewsalarysaleparts.h"
#include "stableviewsalarysales.h"

SPageSalarySales::SPageSalarySales(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalarySales)
{
    ui->setupUi(this);

    ui->tableViewSales->setModel(parentTab->m_sales);
    ui->tableViewSaleParts->setModel(parentTab->m_saleParts);

    connect(ui->tableViewSales, &STableViewSalarySales::pressed, this, &SPageSalarySales::tableSalesRowActivated);
    connect(ui->tableViewSales, &STableViewSalarySales::clicked, this, &SPageSalarySales::tableSalesRowActivated);
    connect(ui->tableViewSales, &STableViewSalarySales::activated, this, &SPageSalarySales::tableSalesRowActivated);
    connect(ui->tableViewSales, &STableViewSalarySales::doubleClicked, this, &SPageSalarySales::tableSalesRowDoubleClicked);
    connect(ui->tableViewSaleParts, &STableViewSalarySales::doubleClicked, this, &SPageSalarySales::tableSalePartsRowDoubleClicked);
    connect(ui->tableViewSales->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SPageSalarySales::tableSalesRowSelected);
}

SPageSalarySales::~SPageSalarySales()
{
    delete ui;
}

void SPageSalarySales::loadDocumentData(const int id)
{
    ui->tableViewSaleParts->setQuery(QString(
                                       "SELECT                                                                       \n"\
                                       "  CONCAT(t2.`articul`, '-', t1.`item_id`) AS 'UID',                          \n"\
                                       "  t2.`name`,                                                                 \n"\
                                       "  t1.`count`,                                                                \n"\
                                       "  t1.`price`,                                                                \n"\
                                       "  @summ:=t1.`count` * t1.`price` AS 'summ',                                  \n"\
                                       "  t1.`sn`,                                                                   \n"\
                                       "  t1.`warranty`,                                                             \n"\
                                       "  @profit:=t1.`count`*(t1.`price`-t2.`in_price`) AS 'profit',                \n"\
                                       "  CAST(@profit*t3.`pay_sale`/100 AS DECIMAL(11,4)) AS 'salary_part'          \n"\
                                       "FROM `store_sales` AS t1                                                     \n"\
                                       "LEFT JOIN `store_items` AS t2                                                \n"\
                                       "  ON t1.`item_id` = t2.`id`                                                  \n"\
                                       "LEFT JOIN `users` AS t3                                                      \n"\
                                       "  ON t1.`user` = t3.`id`                                                     \n"\
                                       "WHERE                                                                        \n"\
                                       "  t1.`document_id` = %1"                                                        \
                                       )
                                       .arg(id)\
                                       , QSqlDatabase::database("connMain"));
    ui->tableViewSaleParts->refresh();
}

void SPageSalarySales::updateModels()
{
    ui->tableViewSales->setQuery(QString(
                                   "SELECT                                                                           \n"\
                                   "  `salary`.`id`,                                                                 \n"\
                                   "  `salary`.`created`,                                                            \n"\
                                   "  `salary`.`client`,                                                             \n"\
                                   "  `salary`.`total`,                                                              \n"\
                                   "  `salary`.`profit`,                                                             \n"\
                                   "  `salary`.`profit`*`users`.`pay_sale`/100 AS 'salary_part'                      \n"\
                                   "FROM (                                                                           \n"\
                                   "  SELECT                                                                         \n"\
                                   "    t1.`id`,                                                                     \n"\
                                   "    t1.`created`,                                                                \n"\
                                   "    CONCAT_WS(' ', t2.surname, t2.`name`, t2.patronymic) AS 'client',            \n"\
                                   "    t1.`total`,                                                                  \n"\
                                   "    SUM(t3.`count`*(t3.`price`-t3.`in_price`)) AS 'profit',                      \n"\
                                   "    t1.`user`                                                                    \n"\
                                   "  FROM `docs` AS `t1`                                                            \n"\
                                   "  LEFT JOIN `clients` AS t2                                                      \n"\
                                   "    ON t1.`dealer` = t2.`id`                                                     \n"\
                                   "  LEFT JOIN `store_sales` AS t3                                                  \n"\
                                   "    ON t1.id = t3.`document_id`                                                  \n"\
                                   "  LEFT JOIN `store_items` AS t4                                                  \n"\
                                   "    ON t3.`item_id` = t4.`id`                                                    \n"\
                                   "  WHERE                                                                          \n"\
                                   "    t1.`type` = 2                                                                \n"\
                                   "    AND t1.`state` = 5                                                           \n"\
                                   "    AND t1.`created` >= '%1'                                                     \n"\
                                   "    AND t1.`created` <= '%2'                                                     \n"\
                                   "    AND t1.`user` = %3                                                           \n"\
                                   "  GROUP BY t1.`id`                                                               \n"\
                                   ") AS `salary`                                                                    \n"\
                                   "LEFT JOIN `users`                                                                \n"\
                                   "  ON `salary`.`user` = `users`.`id`                                              "\
                                   )\
                                   .arg(parentTab->periodBegin())\
                                   .arg(parentTab->periodEnd())\
                                   .arg(parentTab->m_employeeId)\
                                 , QSqlDatabase::database("connMain"));

    ui->tableViewSales->refresh();

    parentTab->setModelUpdatedFlag(SalesModel);
}

void SPageSalarySales::updateWidgets()
{
    m_userModel = parentTab->m_userModel;

    ui->labelPercentPartsValue->setText(QString::number(m_userModel->paySale()));
    ui->labelSalesSummValue->setText(sysLocale.toString(parentTab->m_sales->total(5), 'f', comSettings->value("classic_kassa").toBool()?2:0));
}

void SPageSalarySales::tableSalesRowSelected(const QModelIndex &current, const QModelIndex &prev)
{
    tableSalesRowActivated(current);
}

void SPageSalarySales::tableSalesRowActivated(const QModelIndex &index)
{
    loadDocumentData(parentTab->m_sales->index(index.row(), 0).data().toInt());
}

void SPageSalarySales::tableSalesRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSale(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalarySales::tableSalePartsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSparePart(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

