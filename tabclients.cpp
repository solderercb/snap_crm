#include "appver.h"
#include "tabclients.h"
#include "ui_tabclients.h"
#include "mainwindow.h"
#include "com_sql_queries.h"

tabClients* tabClients::p_instance[] = {nullptr,nullptr};

tabClients::tabClients(bool type, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabClients)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    _type = type;
    userData            = parent->userData;
    permissions         = parent->permissions;
    comSettings         = parent->comSettings;

    ui->tableView->horizontalHeader()->setSectionsMovable(true);  // возможность двигать столбцы (ну шоб как АСЦ было :-) )
    ui->tableView->verticalHeader()->hide();
    clientsTable = new QSqlQueryModel();
//    proxyModel = new QSortFilterProxyModel();
//    proxyModel->setSourceModel(clientsTable);
//    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    ui->tableView->setModel(clientsTable);
    if (type == 1)
    {
        ui->buttonPrint->hide();
        ui->buttonClientNew->hide();
    }

    clientsTypesList = new QStandardItemModel(this);
    clientTypeSelector[0] << new QStandardItem("Все клиенты") << new QStandardItem("0") << new QStandardItem("t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[0] );
    clientTypeSelector[1] << new QStandardItem("Организации") << new QStandardItem("1") << new QStandardItem("t1.`type` = 1 AND t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[1] );
    clientTypeSelector[2] << new QStandardItem("Посредники") << new QStandardItem("2") << new QStandardItem("t1.`is_agent` = 1 AND t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[2] );
    clientTypeSelector[3] << new QStandardItem("Поставщики") << new QStandardItem("3") << new QStandardItem("t1.`is_dealer` = 1 AND t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[3] );
    clientTypeSelector[4] << new QStandardItem("Постоянные клиенты") << new QStandardItem("4") << new QStandardItem("t1.`is_regular` = 1 AND t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[4] );
    clientTypeSelector[5] << new QStandardItem("Проблемные клиенты") << new QStandardItem("5") << new QStandardItem("t1.`is_bad` = 1 AND t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[5] );
    clientTypeSelector[6] << new QStandardItem("Реализаторы") << new QStandardItem("6") << new QStandardItem("t1.`is_realizator` = 1 AND t1.`state` = 1");
    clientsTypesList->appendRow( clientTypeSelector[6] );
    clientTypeSelector[7] << new QStandardItem("Архивные") << new QStandardItem("7") << new QStandardItem("t1.`state` = 0");
    clientsTypesList->appendRow( clientTypeSelector[7] );
    ui->listViewClientsType->setModel(clientsTypesList);
    ui->listViewClientsType->setModelColumn(0);
    if (userData->value("prefer_regular").toBool())
        ui->listViewClientsType->setCurrentIndex(clientsTypesList->index(4, 0));
    else
        ui->listViewClientsType->setCurrentIndex(clientsTypesList->index(0, 0));    // по умолчанию выбираем одну из категорий; обязательно! иначе будет вылетать при сборке условия в updateTableWidget()

    clientAdTypesList = new QSqlQueryModel(this);
    clientAdTypesList->setQuery(QUERY_SEL_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setModelColumn(0);

    query_static = "SELECT t1.`id`, CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO', t1.`balance`, t1.`repairs`, t1.`purchases`, IF(t1.`type` = 1, 'Ю', '') AS 'type', IFNULL(t2.`phone`, '') AS 'phone' FROM `clients` AS t1 LEFT JOIN `tel` AS t2 ON t1.`id` = t2.`customer` AND t2.`type` = 1"; // default query
//    query_where_static = "";    // default WHERE part of query
//    query_where << query_where_static;
    query_group_static = "t1.`id`";    // default GROUP part of query
    query_group << query_group_static;
    query_order_static = "t1.`id` ASC, t2.`id` ASC";   // default ORDER part of query
    query_order << query_order_static;

    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionMoved(int, int, int)), this, SLOT(tableSectionMoved(int, int, int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int, int, int)), this, SLOT(tableSectionResized(int, int, int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(tableSortingChanged(int, Qt::SortOrder)));

    // ТУТА нужно быть аккуратным! Если в конструкторе MainWindow вызвать функцию-слот создания вкладки tabClients, то получим цикл.
    connect(ui->buttonClientNew,SIGNAL(clicked()), MainWindow::getInstance(), SLOT(createTabRepairNew()));

    updateTableWidget();
}

tabClients::~tabClients()
{
    p_instance[this->_type] = nullptr;   // Обязательно блять!
    clientsTable->clear();
    delete ui;
}

tabClients* tabClients::getInstance(bool type, MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabClients(type, parent);
    return p_instance[type];
}

void tabClients::lineEditSearchSetFocus()
{
//    ui->lineEditSearch->setFocusPolicy(Qt::StrongFocus);
    ui->lineEditSearch->setFocus();
}

void tabClients::updateTableWidget()
{
    qDebug() << "updateTableWidget()";
    query.clear();

    /* Собираем условия для запроса */
    query_where.clear();
    if (query_where_static.length() > 0)    // если предустановлен дефолтный фильтр
        query_where << query_where_static;
    query_where << clientsTypesList->item(ui->listViewClientsType->currentIndex().row(), 2)->text();  // добавляем условие для выбранной категории клиентов
    if (ui->comboBoxClientAdType->currentIndex() > 0 )
//        qDebug() << "clientsAdTypesList->index(ui->comboBoxAdvertising->currentIndex(), 1).data() = " << clientsAdTypesList->index(ui->comboBoxAdvertising->currentIndex(), 1).data().toString();
        query_where << QString("`visit_source` = %1").arg(clientAdTypesList->index(ui->comboBoxClientAdType->currentIndex(), 1).data().toString());
    if (ui->lineEditSearch->text().length() > 0)    // только если строка поиска не пуста
        query_where << QString("(LCASE(CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) REGEXP LCASE('%1') OR t1.`id` = '%1' OR t2.`phone` REGEXP '%1' OR t2.`phone_clean` REGEXP '%1')").arg(ui->lineEditSearch->text());

    query << query_static << (query_where.count()>0?"WHERE " + query_where.join(" AND "):"") << "GROUP BY" << query_group.join(", ") << "ORDER BY" << query_order.join(", ");
//    qDebug() << query.join(' ');
    clientsTable->setQuery(query.join(' '), QSqlDatabase::database("connMain"));
    ui->labelClientsCounter->setText(QString::number(clientsTable->rowCount()));
}

void tabClients::clientTypeChanged(QModelIndex index)
{
//    qDebug() << "clientTypeChanged(QModelIndex): item1 =" << clientsTypesList->index(index.row(), 0).data() << ", item2 = " << clientsTypesList->index(index.row(), 1).data() << "item3 = " << clientsTypesList->index(index.row(), 2).data();
    updateTableWidget();
}

void tabClients::clientAdvertisingChanged(int index)
{
    qDebug() << "SLOT clientAdvertisingChanged(int index), index = " << ui->comboBoxClientAdType->currentIndex();
    updateTableWidget();
}

void tabClients::tableItemDoubleClick(QModelIndex item)
{
    emit doubleClicked(clientsTable->index(item.row(), 0).data().toInt());
    if (_type == 1)
        deleteLater();
}

void tabClients::lineEditSearchTextChanged(QString search_str)
{
//    qDebug() << "SLOT tabClients::lineEditSearchTextChanged(QString search_str), search_str = " << search_str;
    updateTableWidget();
}

void tabClients::lineEditSearchReturnPressed()
{

}

/* В слоте будем сохранять настроенное пользователем положение столбца */
void tabClients::tableSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    qDebug() << "SLOT tableSectionMoved(int, int, int)";
}

/* В слоте будем сохранять настроенную пользователем ширину столбца */
void tabClients::tableSectionResized(int logicalIndex, int oldSize, int newSize)
{
    qDebug() << "SLOT tableSectionResized(int, int, int)";
}

/*
    Сортировка будет выполняться посредством запроса к БД
    При больших размерах таблицы (например, "Все клиенты" — ~7000 строк) это сильно быстрее чем QSortFilterProxyModel
*/
void tabClients::tableSortingChanged(int index, Qt::SortOrder order)
{
    QString order_str;

//    qDebug() << "SLOT tableSortingChanged(int, Qt::SortOrder)";
    query_order.clear();

    if (order == Qt::AscendingOrder)
        order_str = " ASC";
    else
        order_str = " DESC";

    switch (index) {
        case 0: query_order << "t1.`id`" + order_str; break;
        case 1: query_order << "CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)" + order_str; break;
        case 2: query_order << "t1.`balance`" + order_str; break;
        case 3: query_order << "t1.`repairs`" + order_str; break;
        case 4: query_order << "t1.`purchases`" + order_str; break;
        case 5: query_order << "t1.`type`" + order_str; break;
        case 6: query_order << "t2.`phone`" + order_str; break;
    }

    updateTableWidget();
}

void tabClients::togglePropertiesPanel()
{

}

