#include "tabrepairs.h"
#include "ui_tabrepairs.h"

tabRepairs* tabRepairs::p_instance = nullptr;

tabRepairs::tabRepairs(bool type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairs)
{
    ui->setupUi(this);
    this->setWindowTitle("Ремонты");
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->tableView->horizontalHeader()->setSectionsMovable(true);  // возможность двигать столбцы (ну шоб как АСЦ было :-) )
    ui->tableView->verticalHeader()->hide();
    repairs_table = new repairsTableModel();
    ui->tableView->setModel(repairs_table);
    updateTableWidget();
}

tabRepairs::~tabRepairs()
{
    delete ui;
    p_instance = nullptr;   // Обязательно блять!
}

void tabRepairs::updateTableWidget()
{
//    qDebug() << "updateTable func";

    repairs_table->setQuery("SELECT\
                    CONCAT_WS(' ', IF(`is_warranty`, 'Г', ''), IF(`is_repeat`, 'П', ''), IF(`express_repair`, 'С', ''), IF(`informed_status`, '*', '')) AS 'marks',\
                    t1.`id`,\
                    `Title`,\
                    CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',\
                    CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\
                    `serial_number`,\
                    `office`,\
                    `manager`,\
                    `master`,\
                    `in_date`,\
                    t1.`state`,\
                    t1.`state`,\
                    `new_state`,\
                    `user_lock`,\
                    `lock_datetime`,\
                    `quick_repair`,\
                    `box`,\
                    `repair_cost`,\
                    `fault`,\
                    `thirs_party_sc`,\
                    `last_save`,\
                    `last_status_changed`,\
                    `warranty_days`,\
                    `color`,\
                    `early`,\
                    `ext_early`,\
                    `issued_msg`,\
                    `vendor_id`,\
                    `termsControl`,\
                    `Hidden`\
                  FROM `workshop` AS t1 LEFT JOIN `devices` AS t2 ON t1.`type` = t2.`id` LEFT JOIN `device_makers` AS t3 ON t1.maker = t3.`id` LEFT JOIN `device_models` AS t4 ON t1.model = t4.`id` LEFT JOIN `clients` AS t5 ON t1.`client` = t5.`id` WHERE `out_date` IS NULL AND `company` = 1 ORDER BY `id` DESC;", QSqlDatabase::database("connMain"));

//    repairs_table->setHeaderData(0, Qt::Horizontal, tr("Name"));
//    repairs_table->setHeaderData(1, Qt::Horizontal, tr("Salary"));
}

void tabRepairs::tableItemDoubleClick(QModelIndex item)
{
    emit doubleClicked(repairs_table->index(item.row(), 1).data().toInt());
}

void tabRepairs::lineEditSearchTextChanged(QString)
{

}

void tabRepairs::lineEditSearchReturnPressed()
{

}

tabRepairs* tabRepairs::getInstance(QWidget *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
if( !p_instance )
  p_instance = new tabRepairs(0, parent);
return p_instance;
}

