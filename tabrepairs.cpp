#include "tabrepairs.h"
#include "ui_tabrepairs.h"
#include "mainwindow.h"

tabRepairs* tabRepairs::p_instance[] = {nullptr,nullptr};

tabRepairs::tabRepairs(bool type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairs)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    _type = type;
    ui->tableView->horizontalHeader()->setSectionsMovable(true);  // возможность двигать столбцы (ну шоб как АСЦ было :-) )
    ui->tableView->verticalHeader()->hide();
    repairs_table = new repairsTableModel();
    ui->tableView->setModel(repairs_table);
    updateTableWidget();
    ui->tableView->horizontalHeader()->moveSection(10,4);
    if (type == 1)
    {
        ui->buttonPrint->hide();
        ui->buttonRepairNew->hide();
    }
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionMoved(int, int, int)), this, SLOT(tableSectionMoved(int, int, int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int, int, int)), this, SLOT(tableSectionResized(int, int, int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(tableSortingChanged(int, Qt::SortOrder)));

    // ТУТА нужно быть аккуратным! Если в конструкторе MainWindow вызвать функцию-слот создания вкладки tabRepairs, то получим цикл.
    connect(ui->buttonRepairNew,SIGNAL(clicked()), MainWindow::getInstance(), SLOT(createTabRepairNew()));
}

tabRepairs::~tabRepairs()
{
    p_instance[this->_type] = nullptr;   // Обязательно блять!
    delete ui;
}

tabRepairs* tabRepairs::getInstance(bool type, QWidget *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabRepairs(type, parent);
    return p_instance[type];
}

void tabRepairs::updateTableWidget()
{
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

        ui->tableView->horizontalHeader()->hideSection(11); // прячем столбец с кодом статуса
//    repairs_table->setHeaderData(0, Qt::Horizontal, tr("Name"));
//    repairs_table->setHeaderData(1, Qt::Horizontal, tr("Salary"));
}

void tabRepairs::tableItemDoubleClick(QModelIndex item)
{
    emit doubleClicked(repairs_table->index(item.row(), 1).data().toInt());
    if (_type == 1)
        deleteLater();
}

void tabRepairs::lineEditSearchTextChanged(QString)
{

}

void tabRepairs::lineEditSearchReturnPressed()
{

}

/* В слоте будем сохранять настроенное пользователем положение столбца */
void tabRepairs::tableSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    qDebug() << "Slot tableSectionMoved(int, int, int)";
}

/* В слоте будем сохранять настроенную пользователем ширину столбца */
void tabRepairs::tableSectionResized(int logicalIndex, int oldSize, int newSize)
{
    qDebug() << "Slot tableSectionResized(int, int, int)";
}

void tabRepairs::tableSortingChanged(int column, Qt::SortOrder order)
{
    qDebug() << "Slot tableSortingChanged(int, Qt::SortOrder)";
    repairs_table->sort(column, order);
}

