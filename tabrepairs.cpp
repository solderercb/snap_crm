#include "global.h"
#include "appver.h"
#include "tabrepairs.h"
#include "ui_tabrepairs.h"
#include "com_sql_queries.h"
//#include "mainwindow.h"

tabRepairs* tabRepairs::p_instance[] = {nullptr,nullptr};

tabRepairs::tabRepairs(bool type, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepairs)
{
    userActivityLog->appendRecord(tr("Navigation Ремонты"));

    ui->setupUi(this);

    filterSettings = new QMap<QString, int>;
    repairTableFilterMenu *widgetAction = new repairTableFilterMenu(this);
    ui->buttonShowParamsPopupMenu->addAction(widgetAction);

    this->setAttribute(Qt::WA_DeleteOnClose);
    _type = type;
    ui->tableView->horizontalHeader()->setSectionsMovable(true);  // возможность двигать столбцы (ну шоб как АСЦ было :-) )
    ui->tableView->verticalHeader()->hide();
    repairs_table = new repairsTableModel();
    ui->tableView->setModel(repairs_table);
    if (type == 1)
    {
        ui->buttonPrint->hide();
        ui->buttonRepairNew->hide();
    }

    QSqlQueryModel *clientsModel = new QSqlQueryModel();
    clientsModel->setQuery("SELECT 1;", QSqlDatabase::database("connMain"));
    widgetAction->setComboBoxOfficeModel(officesModel);
    widgetAction->setComboBoxStatusModel(statusesModel);
    widgetAction->setComboBoxEmployeeModel(usersModel);
    widgetAction->setComboBoxClientModel(clientsTypesList);
//    widgetAction->setComboBoxXModel(repairModel);
    widgetAction->setFilterSettings(filterSettings);

    connect(widgetAction, SIGNAL(hidden()), this, SLOT(updateTableWidget()));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionMoved(int,int,int)), this, SLOT(tableSectionMoved(int,int,int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this, SLOT(tableSectionResized(int,int,int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(tableSortingChanged(int,Qt::SortOrder)));
    connect(ui->buttonRepairNew, &QPushButton::clicked, this, &tabRepairs::buttonRepairNewClicked);

    // ТУТА нужно быть аккуратным! Если в конструкторе MainWindow вызвать функцию-слот создания вкладки tabRepairs, то получим цикл.
    connect(ui->buttonRepairNew,SIGNAL(clicked()), MainWindow::getInstance(), SLOT(createTabRepairNew()));

    tableUpdateDelay = new QTimer();
    QObject::connect(tableUpdateDelay, SIGNAL(timeout()), this, SLOT(updateTableWidget()));
    tableUpdateDelay->setSingleShot(true);

    updateTableWidget();
}

tabRepairs::~tabRepairs()
{
    userActivityLog->updateActivityTimestamp();

    p_instance[this->_type] = nullptr;   // Обязательно блять!
    delete ui;
}

QString tabRepairs::tabTitle()
{
    if(_type)
        return tr("Выбрать ремонт");
    else
        return tr("Ремонты");
}

tabRepairs* tabRepairs::getInstance(bool type, MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabRepairs(type, parent);
    return p_instance[type];
}

/* Обновление модели данных для таблицы ремонтов с учетом фильтров.
 * При первом запуске модель содержит только текущие ремонты.
 * При включении ЛЮБОГО фильтра (будь то текстовый поиск или фильтр по
 * категориям из всплывающего меню) будут отображаться в том числе и
 * выданные ремонты
*/
void tabRepairs::updateTableWidget()
{
//    qDebug() << "tabRepairs::updateTableWidget()";
    // TODO: нужно уйти от жестко заданных имён/алиасов таблиц
    // TODO: компания/офис

    query.clear();
    query_where.clear();

    /* Собираем условия для запроса */
    if (ui->lineEditSearch->text().length() > 0)    // если строка поиска не пуста
    {
        query_where << QString("(t1.`id` LIKE '%%1%' OR LCASE(t1.`title`) REGEXP LCASE('%1') OR LCASE(t1.`serial_number`) REGEXP LCASE('%1') OR LCASE(CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic)) REGEXP LCASE('%1')) AND t1.`company` = 1").arg(ui->lineEditSearch->text());
    }
    if(filterSettings->contains("status") && filterSettings->value("status") >= 0)
    {
        query_where << QString("t1.`state` = %1").arg(statusesModel->item(filterSettings->value("status"), 1)->text());
    }
    if(filterSettings->contains("office") && filterSettings->value("office") >= 0)
    {
        query_where << QString("`office` = %1").arg(officesModel->record(filterSettings->value("office")).value("id").toString());
    }
    if(filterSettings->contains("employee") && filterSettings->value("employee") >= 0)
    {
//        query_where << QString("(`manager` = %1 OR `master` = %1)").arg(usersModel->record(filterSettings->value("employee")).value("id").toString());
        query_where << QString("`master` = %1").arg(usersModel->record(filterSettings->value("employee")).value("id").toString());
    }
    if(filterSettings->contains("client") && filterSettings->value("client") >= 0)
    {
        query_where << "t5." + clientsTypesList->item(filterSettings->value("client"), 2)->text();
    }
    if ( !userDbData->value("display_out").toBool() && query_where.count() == 0 )    // дефолтный фильтр, если не настроен пользователем
        query_where << "`out_date` IS NULL AND `company` = 1";

    query_group << "`id`";    // default GROUP part of query
    query_order << "`id` DESC";   // default ORDER part of query

    query << QUERY_SEL_WORKSHOP_STATIC\
          << (query_where.count()>0?"\nWHERE " + query_where.join(" AND "):"")\
          << (query_group.count()>0?"\nGROUP BY " + query_group.join(", "):"")\
          << (query_order.count()>0?"\nORDER BY " + query_order.join(", "):"");
//    qDebug() << query.join(' ');
//    clientsTable->setQuery(query.join(' '), QSqlDatabase::database("connMain"));
//    ui->labelClientsCounter->setText(QString::number(clientsTable->rowCount()));

    repairs_table->setQuery(query.join(' '), QSqlDatabase::database("connMain"));

    ui->tableView->horizontalHeader()->hideSection(12); // прячем столбец с кодом статуса
//    ui->tableView->horizontalHeader()->moveSection(10,4);   // это просто эксперимент (работает)
//    repairs_table->setHeaderData(0, Qt::Horizontal, tr("Name"));
//    repairs_table->setHeaderData(1, Qt::Horizontal, tr("Salary"));
}

void tabRepairs::tableItemDoubleClick(QModelIndex item)
{
    emit doubleClicked(repairs_table->index(item.row(), 1).data().toInt());
    if (_type == 1)
    {
        emit activateCaller(callerPtr);
        deleteLater();
    }
}

void tabRepairs::lineEditSearchTextChanged(QString)
{   // задержка поиска; запрос к базе будет выполняться после каждого введённого символа и при быстром наборе текста прога тормозит
    tableUpdateDelay->stop();
    tableUpdateDelay->start(350);
}

void tabRepairs::lineEditSearchReturnPressed()
{
    QString s = ui->lineEditSearch->text();
    int pos = 0;
    QSqlQuery* num_validator = new QSqlQuery(QSqlDatabase::database("connMain"));   // запрос к БД для проверки валидности введённого номера ремонта
    QRegularExpression re("\\d+");  // только цифры
    QRegularExpressionValidator v(re, 0);

    if(v.validate(s, pos) == QValidator::Acceptable)   // если введены только цифры
    {
        num_validator->exec(QString("SELECT 1 FROM workshop WHERE `id` = %1").arg(s));  // проверяем если ли в таблице workshop ремонт с таким номером
        if (num_validator->size() > 0)  // если возвращено не ноль строк
        {
            emit doubleClicked(s.toInt());  // посылаем сигнал открытия карты ремонта или вставки номера ремонта в поле формы приёма в ремонт
            if (_type == 1)
                deleteLater();
        }
    }
    delete num_validator;   // подчищаем за собой
}

/* В слоте будем сохранять настроенное пользователем положение столбца */
void tabRepairs::tableSectionMoved(int, int, int)
{
    qDebug() << "Slot tableSectionMoved(int, int, int)";
}

/* В слоте будем сохранять настроенную пользователем ширину столбца */
void tabRepairs::tableSectionResized(int, int, int)
{
//    qDebug() << "Slot tableSectionResized(int, int, int)";
}

void tabRepairs::tableSortingChanged(int column, Qt::SortOrder order)
{
    qDebug() << "Slot tableSortingChanged(int, Qt::SortOrder)";
    repairs_table->sort(column, order);
}

