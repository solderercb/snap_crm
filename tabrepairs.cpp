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
    m_type = type;

    tableUpdateDelay = new QTimer();
    repairs_table = new STableRepairsModel();
    if(comSettings->isCartridgeRepairEnabled)
        cartridges_table = new STableRepairsModel();

    ui->tableView->setModel(repairs_table);
    if (type == 1)
        ui->buttonPrint->hide();

    if(!permissions->receptDevices || type == 1)
        ui->buttonRepairNew->hide();

    widgetAction->setComboBoxOfficeModel(officesModel);
    widgetAction->setComboBoxStatusModel(statusesModel);
    widgetAction->setComboBoxEmployeeModel(usersModel);
    widgetAction->setComboBoxClientModel(clientsTypesList);
//    widgetAction->setComboBoxXModel(repairModel);
    widgetAction->setFilterSettings(filterSettings);

    connect(widgetAction, SIGNAL(hidden()), this, SLOT(filterMenuClosed()));
    connect(ui->buttonRepairNew, &QPushButton::clicked, this, &tabRepairs::buttonRepairNewClicked);

    // ТУТА нужно быть аккуратным! Если в конструкторе MainWindow вызвать функцию-слот создания вкладки tabRepairs, то получим цикл.
    connect(ui->buttonRepairNew,SIGNAL(clicked()), MainWindow::getInstance(), SLOT(createTabRepairNew()));

    QObject::connect(tableUpdateDelay, SIGNAL(timeout()), this, SLOT(autorefreshTable()));
    tableUpdateDelay->setSingleShot(true);

    updateWidgets();
    refreshTable();
}

tabRepairs::~tabRepairs()
{
    userActivityLog->updateActivityTimestamp();

    p_instance[this->m_type] = nullptr;   // Обязательно блять!
    delete ui;
    delete tableUpdateDelay;
}

QString tabRepairs::tabTitle()
{
    if(m_type)
        return tr("Выбрать ремонт");
    else
        return tr("Ремонты");
}

void tabRepairs::updateWidgets()
{
    if(!comSettings->isCartridgeRepairEnabled)
    {
        ui->switchTableMode->hide();
        ui->labelTableMode->hide();
        tableModeChanged(NoCartridges);
    }
    else
    {
        ui->switchTableMode->show();
        ui->labelTableMode->show();
        tableModeChanged(NoCartridges);
//        tableModeChanged(userLocalData-><TODO: defaultTableMode>.value);
    }
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
 * preserveScrollPos - флаг указывающий на необходимость сохранить текущюю позицию прокрутки, по умолчанию сохр.
 * preserveSelection - флаг указывающий на необходимость сохранить текущее выделение, по умолчанию не сохр.
*/
void tabRepairs::refreshTable(bool preserveScrollPos, bool preserveSelection)
{
    // TODO: нужно уйти от жестко заданных имён/алиасов таблиц
    // TODO: компания/офис

    ui->tableView->setQuery(QUERY_SEL_WORKSHOP_STATIC, QSqlDatabase::database("connMain"));
    ui->tableView->setUniqueIdColumn(1);
    FilterList l1;
    l1.op = FilterList::And;

    l1.fields.append(STableViewBase::initFilterField("t2.`refill`", FilterField::Equals, ui->switchTableMode->isChecked()));
    l1.fields.append(STableViewBase::initFilterField("`company`", FilterField::Equals, 1));
    if ( !userDbData->displayOut && ui->lineEditSearch->text().isEmpty() )
        l1.fields.append(STableViewBase::initFilterField("`out_date`", FilterField::Null, ""));
    if(filterSettings->contains("status") && filterSettings->value("status") >= 0)
        l1.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::Equals, statusesModel->databaseIDByRow(filterSettings->value("status"))));
    if(filterSettings->contains("office") && filterSettings->value("office") >= 0)
        l1.fields.append(STableViewBase::initFilterField("t1.`office`", FilterField::Equals, officesModel->databaseIDByRow(filterSettings->value("office"))));
    if(filterSettings->contains("employee") && filterSettings->value("employee") >= 0)
        l1.fields.append(STableViewBase::initFilterField("t1.`master`", FilterField::Equals, usersModel->databaseIDByRow(filterSettings->value("employee"))));
    if(filterSettings->contains("client") && filterSettings->value("client") >= 0)
        l1.fields.append(STableViewBase::initFilterField("t5." + clientsTypesList->item(filterSettings->value("client"), 2)->text(), FilterField::NoOp, ""));

    FilterList l2;
    l2.op = FilterList::Or;
    l2.fields.append(STableViewBase::initFilterField("t1.`id`", FilterField::RegExp, ui->lineEditSearch->text()));
    l2.fields.append(STableViewBase::initFilterField("t1.`title`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l2.fields.append(STableViewBase::initFilterField("t1.`serial_number`", FilterField::RegExp, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l2.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t5.`surname`, t5.`name`, t5.`patronymic`)", FilterField::RegExp, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l1.childs.append(l2);

    query_group.clear();
    query_group << "`id`";    // default GROUP part of query

    ui->tableView->setFilter(l1);
    ui->tableView->setGrouping(query_group);
    ui->tableView->refresh(preserveScrollPos, preserveSelection);

    tableUpdateDelay->start(userDbData->refreshTime*1000);
}

void tabRepairs::autorefreshTable()
{
    refreshTable(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
}

void tabRepairs::tableItemDoubleClick(QModelIndex item)
{
    emit doubleClicked(repairs_table->record(item.row()).value("id").toInt());
    if (m_type == 1)
    {
        emit activateCaller(callerPtr);
        deleteLater();
    }
}

/*  Поиск по введённому тексту
 *  Производится с задержкой после ввода для уменьшения кол-ва запросов и исключения тормозов интерфейса
*/
void tabRepairs::lineEditSearchTextChanged(QString)
{
    ui->tableView->clearSelection();
    ui->tableView->resetVScrollPos();
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
            if (m_type == 1)
                deleteLater();
        }
    }
    delete num_validator;   // подчищаем за собой
}

void tabRepairs::tableModeChanged(bool mode)
{
    if(mode)
        ui->labelTableMode->setText(tr("Картриджи"));
    else
        ui->labelTableMode->setText(tr("Ремонты"));

    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

void tabRepairs::filterMenuClosed()
{
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

