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
    ui->pushButtonMenu->addAction(widgetAction);

    this->setAttribute(Qt::WA_DeleteOnClose);
    m_tabType = type;

    tableUpdateDelay = new QTimer();
    repairs_table = new STableRepairsModel();
    if(comSettings->isCartridgeRepairEnabled)
        cartridges_table = new STableRepairsModel();

    ui->tableView->setModel(repairs_table);

    if(!permissions->receptDevices || type == 1)
        ui->pushButtonReceipt->hide();

    widgetAction->setComboBoxOfficeModel(officesModel);
    widgetAction->setComboBoxStatusModel(statusesModel);
    widgetAction->setComboBoxEmployeeModel(usersModel);
    widgetAction->setComboBoxClientModel(clientsTypesList);
//    widgetAction->setComboBoxXModel(repairModel);
    widgetAction->setFilterSettings(filterSettings);

    if(m_tabType == SelectRepair)
        ui->switchTableMode->setChecked(STableViewRepairs::ModeRepairs);
    else
        ui->switchTableMode->setChecked((userDbData->defWsFilter == WorkshopFilter::CartridgesOnly)?STableViewRepairs::ModeCartridges:STableViewRepairs::ModeRepairs);

    connect(widgetAction, SIGNAL(hidden()), this, SLOT(filterMenuClosed()));
    connect(ui->pushButtonReceipt, &QPushButton::clicked, this, &tabRepairs::buttonReceptClicked);
    connect(ui->pushButtonRefill, &QPushButton::clicked, this, &tabRepairs::buttonRefillClicked);
    connect(ui->pushButtonIssue, &QPushButton::clicked, this, &tabRepairs::buttonIssueClicked);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &tabRepairs::buttonRefreshClicked);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionMoved, this, &tabRepairs::tableLayoutChanged);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &tabRepairs::tableLayoutChanged);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &tabRepairs::tableSelectionChanged);
    connect(tableUpdateDelay, SIGNAL(timeout()), this, SLOT(autorefreshTable()));
    tableUpdateDelay->setSingleShot(true);

    refreshTable();
    updateWidgets();
}

tabRepairs::~tabRepairs()
{
    p_instance[this->m_tabType] = nullptr;   // Обязательно блять!
    delete ui;
    delete tableUpdateDelay;
}

QString tabRepairs::tabTitle()
{
    if(m_tabType == Type::SelectRepair)
        return tr("Выбрать ремонт");
    else
        return tr("Ремонты");
}

void tabRepairs::updateWidgets()
{
    bool visibility = m_tabType == Type::Workshop;
    bool hasSelection = ui->tableView->selectionModel()->hasSelection();

    ui->switchTableMode->setVisible(visibility);
    ui->labelTableMode->setVisible(visibility);
    ui->pushButtonReceipt->setVisible(visibility);
    ui->pushButtonRefill->setVisible(visibility && (ui->switchTableMode->isChecked() == STableViewRepairs::ModeCartridges));
    ui->pushButtonRefill->setEnabled(hasSelection);
    ui->pushButtonIssue->setVisible(visibility);
    ui->pushButtonIssue->setEnabled(ui->tableView->selectedCanBeIssued());
    ui->pushButtonPrint->setVisible(visibility);
    ui->pushButtonRefresh->setVisible(true);
    ui->pushButtonMenu->setVisible(true);

    if(ui->switchTableMode->isChecked() == STableViewRepairs::ModeCartridges)
        ui->labelTableMode->setText(tr("Картриджи"));
    else
        ui->labelTableMode->setText(tr("Ремонты"));
}

tabRepairs* tabRepairs::getInstance(bool type, MainWindow *parent)   // singleton: вкладка каждого типа может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabRepairs(type, parent);
    return p_instance[type];
}

void tabRepairs::refreshIfTabExists()
{
    tabRepairs *repairsTab = nullptr;

    repairsTab = MainWindow::getInstance()->findChild<tabRepairs*>();
    if(repairsTab)
    {
        repairsTab->refreshTable();
    }
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

    l1.fields.append(STableViewBase::initFilterField("t2.`refill`", FilterField::Equals, (ui->switchTableMode->isChecked() == STableViewRepairs::ModeCartridges)));
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

    if(userDbData->autoRefreshWorkspace)
        tableUpdateDelay->start(userDbData->refreshTime*1000);
}

void tabRepairs::autorefreshTable()
{
    refreshTable(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
}

void tabRepairs::tableItemDoubleClick(QModelIndex item)
{
    int id = repairs_table->record(item.row()).value("id").toInt();
    if(ui->switchTableMode->isChecked() == STableViewRepairs::ModeRepairs)
        emit doubleClickRepair(id);
    else
    {
        QList<int> *list = new QList<int>();
        list->append(id);
        emit cartridgesRefill(list);
    }

    if (m_tabType == Type::SelectRepair)
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
            if(ui->switchTableMode->isChecked() == STableViewRepairs::ModeRepairs)
                emit doubleClickRepair(s.toInt());
            if (m_tabType == Type::SelectRepair)
                deleteLater();
        }
    }
    delete num_validator;   // подчищаем за собой
}

void tabRepairs::tableModeChanged(bool mode)
{
    ui->tableView->setMode(mode);
    buttonRefreshClicked();
}

void tabRepairs::filterMenuClosed()
{
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

/*  Перезапуск таймера автообновления
 *  После изменения размера или положения столбца, нужно сначала сохранить новые параметры и только потом обновлять таблицу
*/
void tabRepairs::tableLayoutChanged(int, int, int)
{
    if(userDbData->autoRefreshWorkspace)
        tableUpdateDelay->start(1200);  // сохранение параметров происходит с задержкой 1000мс, автообновление чуть позже
}

void tabRepairs::buttonReceptClicked()
{
    if(ui->switchTableMode->isChecked() == STableViewRepairs::ModeCartridges)
        emit receptCartridges();
    else
        emit receptRepair();
}

void tabRepairs::buttonRefillClicked()
{
    if(!ui->tableView->selectionModel()->hasSelection())
        return;

    emit cartridgesRefill(ui->tableView->selectedRepairsList());

}

void tabRepairs::buttonIssueClicked()
{
    createDialogIssue();
}

void tabRepairs::buttonRefreshClicked()
{
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
    updateWidgets();
}

void tabRepairs::tableSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    updateWidgets();
}

void tabRepairs::createDialogIssue()
{
    QList<SRepairModel*> list;
    QList<int> *idsList = ui->tableView->selectedRepairsList();
    SRepairModel *repair;
    SSaleTableModel *worksAndPartsModel;

    for(int i = 0; i < idsList->count(); i++)
    {
        repair = new SRepairModel(idsList->at(i));
        worksAndPartsModel = new SSaleTableModel(repair);
        worksAndPartsModel->repair_loadTable(idsList->at(i));
        repair->setWorksAndPartsModel(worksAndPartsModel);
        list.append(repair);
    }

    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    connect(m_dialogIssue, &SDialogIssueRepair::onDelete, this, &tabRepairs::closeDialogIssue);

    delete idsList;
}

void tabRepairs::closeDialogIssue()
{
    m_dialogIssue->deleteRepairModels();
    refreshTable(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionReset);
}

