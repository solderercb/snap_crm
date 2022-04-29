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
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;
    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userData->value("id").toString()));
    QUERY_EXEC(query, nDBErr)(QUERY_INS_USER_ACTIVITY(QString("Navigation Ремонты")));
    QUERY_COMMIT_ROLLBACK(query, nDBErr);
    delete query;

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
    updateTableWidget();
    ui->tableView->horizontalHeader()->moveSection(10,4);
    if (type == 1)
    {
        ui->buttonPrint->hide();
        ui->buttonRepairNew->hide();
    }

    query_static = QUERY_SEL_WORKSHOP_STATIC;     // default query
    query_where_static = "`out_date` IS NULL AND `company` = 1";    // default WHERE part of query
    query_where << query_where_static;
    query_group_static = "t1.`id`";    // default GROUP part of query
    query_group << query_group_static;
    query_order_static = "t1.`id` DESC";   // default ORDER part of query
    query_order << query_order_static;

    QSqlQueryModel *clientsModel = new QSqlQueryModel();
    clientsModel->setQuery("SELECT 1;", QSqlDatabase::database("connMain"));
    widgetAction->setComboBoxOfficeModel(officesModel);
    widgetAction->setComboBoxStatusModel(statusesModel);
    widgetAction->setComboBoxEmployeeModel(usersModel);
    widgetAction->setComboBoxClientModel(clientsTypesList);
//    widgetAction->setComboBoxXModel(repairModel);
    widgetAction->setFilterSettings(filterSettings);

    connect(widgetAction, SIGNAL(hidden()), this, SLOT(filterMenuClosed()));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionMoved(int,int,int)), this, SLOT(tableSectionMoved(int,int,int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this, SLOT(tableSectionResized(int,int,int)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(tableSortingChanged(int,Qt::SortOrder)));
    connect(ui->buttonRepairNew, &QPushButton::clicked, this, &tabRepairs::buttonRepairNewClicked);

    // ТУТА нужно быть аккуратным! Если в конструкторе MainWindow вызвать функцию-слот создания вкладки tabRepairs, то получим цикл.
    connect(ui->buttonRepairNew,SIGNAL(clicked()), MainWindow::getInstance(), SLOT(createTabRepairNew()));

    updateTableWidget();
}

tabRepairs::~tabRepairs()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;
    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userData->value("id").toString()));
    QUERY_COMMIT_ROLLBACK(query, nDBErr);
    delete query;

    p_instance[this->_type] = nullptr;   // Обязательно блять!
    delete ui;
}

tabRepairs* tabRepairs::getInstance(bool type, MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabRepairs(type, parent);
    return p_instance[type];
}

void tabRepairs::updateTableWidget()
{
//    qDebug() << "tabRepairs::updateTableWidget()";
    query.clear();

    /* Собираем условия для запроса */
    query_where.clear();
    if (query_where_static.length() > 0)    // если предустановлен дефолтный фильтр
        query_where << query_where_static;
//    query_where << clientsTypesList->item(ui->listViewClientsType->currentIndex().row(), 2)->text();  // добавляем условие для выбранной категории клиентов
//    if (ui->comboBoxClientAdType->currentIndex() > 0 )
////        qDebug() << "clientsAdTypesList->index(ui->comboBoxAdvertising->currentIndex(), 1).data() = " << clientsAdTypesList->index(ui->comboBoxAdvertising->currentIndex(), 1).data().toString();
//        query_where << QString("`visit_source` = %1").arg(clientAdTypesList->index(ui->comboBoxClientAdType->currentIndex(), 1).data().toString());
//    if (ui->lineEditSearch->text().length() > 0)    // только если строка поиска не пуста
//        query_where << QString("(LCASE(CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) REGEXP LCASE('%1') OR t1.`id` = '%1' OR t2.`phone` REGEXP '%1' OR t2.`phone_clean` REGEXP '%1')").arg(ui->lineEditSearch->text());

    query << query_static << (query_where.count()>0?"WHERE " + query_where.join(" AND "):"") << (query_group.count()>0?"GROUP BY " + query_group.join(", "):"") << (query_group.count()>0?"ORDER BY " + query_order.join(", "):"");
//    qDebug() << query.join(' ');
//    clientsTable->setQuery(query.join(' '), QSqlDatabase::database("connMain"));
//    ui->labelClientsCounter->setText(QString::number(clientsTable->rowCount()));

    repairs_table->setQuery(query.join(' '), QSqlDatabase::database("connMain"));

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

void tabRepairs::filterMenuClosed()
{
//    qDebug() << *filterSettings;
}

void tabRepairs::lineEditSearchTextChanged(QString)
{

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
void tabRepairs::tableSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    qDebug() << "Slot tableSectionMoved(int, int, int)";
}

/* В слоте будем сохранять настроенную пользователем ширину столбца */
void tabRepairs::tableSectionResized(int logicalIndex, int oldSize, int newSize)
{
//    qDebug() << "Slot tableSectionResized(int, int, int)";
}

void tabRepairs::tableSortingChanged(int column, Qt::SortOrder order)
{
    qDebug() << "Slot tableSortingChanged(int, Qt::SortOrder)";
    repairs_table->sort(column, order);
}

