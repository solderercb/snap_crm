#include "global.h"
#include "appver.h"
#include "tabrepairs.h"
#include "ui_tabrepairs.h"
#include "com_sql_queries.h"
#include "tabprintdialog.h"

tabRepairs* tabRepairs::p_instance[] = {nullptr,nullptr};

tabRepairs::tabRepairs(bool type, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepairs)
{
    m_tabType = type; // прежде всего
    logUserActivity();

    ui->setupUi(this);
    initTableRepairsMenu();
    initTableCartridgesMenu();

    tableFilter = new SRepairsTableFilterMenu::Filter;
    SRepairsTableFilterMenu *tableFilterMenu = new SRepairsTableFilterMenu(this);
    ui->pushButtonMenu->addAction(tableFilterMenu);

    this->setAttribute(Qt::WA_DeleteOnClose);

    repairs_table = new STableRepairsModel(this);
    if(comSettings->isCartridgeRepairEnabled)
        cartridges_table = new STableRepairsModel(this);

    ui->tableView->setModel(repairs_table);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setQuery(QUERY_SEL_WORKSHOP_STATIC, QSqlDatabase::database("connMain"));
    ui->tableView->setUniqueIdColumn(STableRepairsModel::Columns::Id);

    if(!permissions->receptDevices || type == 1)
        ui->pushButtonReceipt->hide();

    if(companiesModel->rowCount() > 1)
        tableFilterMenu->setComboBoxOfficeModel(officesModel);
    tableFilterMenu->setComboBoRepairStateModel(comSettings->repairStatuses.Model);
    tableFilterMenu->setComboBoxEmployeeModel(usersModel);
    tableFilterMenu->setComboBoxClientsCatModel(clientsTypesList);
//    tableFilterMenu->setComboBoxRfuModel(model);
    tableFilterMenu->setFilterSettings(tableFilter);

    if(m_tabType == SelectRepair)
        ui->switchTableMode->setChecked(STableViewRepairs::ModeRepairs);
    else
        ui->switchTableMode->setChecked((userDbData->defWsFilter == WorkshopFilter::CartridgesOnly)?STableViewRepairs::ModeCartridges:STableViewRepairs::ModeRepairs);

    connect(tableFilterMenu, SIGNAL(hidden()), this, SLOT(filterMenuClosed()));
    connect(ui->pushButtonReceipt, &QPushButton::clicked, this, &tabRepairs::buttonReceptClicked);
    connect(ui->pushButtonRefill, &QPushButton::clicked, this, &tabRepairs::buttonRefillClicked);
    connect(ui->pushButtonIssue, &QPushButton::clicked, this, &tabRepairs::buttonIssueClicked);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &tabRepairs::buttonRefreshClicked);
    connect(ui->pushButtonPrint, &QPushButton::clicked, this, &tabRepairs::buttonPrintClicked);

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &tabRepairs::tableSelectionChanged);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &tabRepairs::menuRequest);
    connect(ui->tableView, &QTableView::clicked, this, &tabRepairs::tableItemClick);
    refreshTable();
    updateWidgets();
}

tabRepairs::~tabRepairs()
{
    p_instance[this->m_tabType] = nullptr;   // Обязательно блять!
    delete ui;
    delete tableFilter;
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
    QString filterActiveIndicatorTooltip;

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

    ui->tableView->setAlternatingRowColors(userDbData->alternateRowsBackground);

    if(tableFilter->filterActive)
    {
        ui->widgetIsFilterActive->setStyleSheet("background-color: rgb(255, 127, 0);");
        filterActiveIndicatorTooltip = tr("Работает фильтр (кнопка Параметры)");
    }
    else
    {
        ui->widgetIsFilterActive->setStyleSheet("");
        filterActiveIndicatorTooltip = QString();
    }
    ui->widgetIsFilterActive->setToolTip(filterActiveIndicatorTooltip);
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

void tabRepairs::initTableRepairsMenu()
{

}

void tabRepairs::initTableCartridgesMenu()
{
    tableCartridgesMenu = new QMenu(this);
    QAction * printReceipt = new QAction(tr("Печать квитанции"), tableCartridgesMenu);
    printReceipt->setProperty("type", MenuCartridgesActions::PrintReceipt);
    connect(printReceipt, &QAction::triggered, this, &tabRepairs::printCartridgeReceiptReports);
    tableCartridgesMenu->addAction(printReceipt);

    QAction * printIssue = new QAction(tr("Печать акта"), tableCartridgesMenu);
    printIssue->setProperty("type", MenuCartridgesActions::PrintIssue);
    connect(printIssue, &QAction::triggered, this, qOverload<>(&tabRepairs::printCartridgeWorksReports));
    tableCartridgesMenu->addAction(printIssue);
}

void tabRepairs::constructQueryClause()
{
    // TODO: нужно уйти от жестко заданных имён/алиасов таблиц
    // TODO: компания/офис

    FilterList list;
    bool displayOutOverride = userDbData->displayOut;

    list.op = FilterList::And;

    if(tableFilter->repairState >= 0)
    {
        QList<int> additionalStates;
        FilterList repairState;
        repairState.op = FilterList::Or;
        repairState.Not = tableFilter->repairStateNot;
        switch(tableFilter->repairState)
        {
            case Global::RepStateIds::Ready:
            case Global::RepStateIds::ReadyNoRepair:
                additionalStates.append(Global::RepStateIds::Ready);
                additionalStates.append(Global::RepStateIds::ReadyNoRepair); break;
            case Global::RepStateIds::Returned:
            case Global::RepStateIds::ReturnedNoRepair:
            case Global::RepStateIds::ReturnedInCredit:
                displayOutOverride = 1;         // при выборе статуса, соответствующего выданному, отображение выданных, заданное в персональных настройках пользователя, игнорируется
                additionalStates.append(Global::RepStateIds::Returned);
                additionalStates.append(Global::RepStateIds::ReturnedNoRepair);
                additionalStates.append(Global::RepStateIds::ReturnedInCredit); break;
            default: additionalStates.append(tableFilter->repairState); break;
        }
        for(auto state : additionalStates)
            repairState.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::Equals, comSettings->repairStatuses[state].Id));
        list.childs.append(repairState);
    }

    list.fields.append(STableViewBase::initFilterField("t2.`refill`", FilterField::Equals, (ui->switchTableMode->isChecked() == STableViewRepairs::ModeCartridges)));
    list.fields.append(STableViewBase::initFilterField("`company`", FilterField::Equals, 1));
    if ( !displayOutOverride && ui->lineEditSearch->text().isEmpty() )
        list.fields.append(STableViewBase::initFilterField("`out_date`", FilterField::Null, ""));
    if(tableFilter->office >= 0)
        list.fields.append(STableViewBase::initFilterField("t1.`office`", FilterField::Equals | tableFilter->officeNot, tableFilter->office));
    if(tableFilter->clientsCat >= 0)
        list.fields.append(STableViewBase::initFilterField("t5." + clientsTypesList->item(tableFilter->clientsCat, 2)->text(), FilterField::NoOp | tableFilter->clientsCatNot, ""));

    if(tableFilter->onlyCurrentUsersRepairs)
    {
        FilterList onlyCurrentUsersRepairs;
        onlyCurrentUsersRepairs.op = FilterList::Or;
        onlyCurrentUsersRepairs.Not = tableFilter->onlyCurrentUsersRepairsNot;
        onlyCurrentUsersRepairs.fields.append(STableViewBase::initFilterField("t1.`current_manager`", FilterField::Equals, userDbData->id));
        onlyCurrentUsersRepairs.fields.append(STableViewBase::initFilterField("t1.`master`", FilterField::Equals, userDbData->id));
        list.childs.append(onlyCurrentUsersRepairs);
    }

    if(tableFilter->employee >= 0)
    {
        FilterList employee;
        employee.op = FilterList::Or;
        employee.Not = tableFilter->employeeNot;
        employee.fields.append(STableViewBase::initFilterField("t1.`current_manager`", FilterField::Equals, tableFilter->employee));
        employee.fields.append(STableViewBase::initFilterField("t1.`master`", FilterField::Equals, tableFilter->employee));
        list.childs.append(employee);
    }

    FilterList period;
    period.op = FilterList::And;
    period.Not = tableFilter->periodNot;
    if(!tableFilter->periodBegin.isEmpty())
        period.fields.append(STableViewBase::initFilterField("t1.`in_date`", FilterField::More, tableFilter->periodBegin));
    if(!tableFilter->periodEnd.isEmpty())
        period.fields.append(STableViewBase::initFilterField("t1.`in_date`", FilterField::Less, tableFilter->periodEnd));
    list.childs.append(period);

    if(!ui->lineEditSearch->text().isEmpty())
    {
        QString what = ui->lineEditSearch->text();
        FilterList searchText;
        searchText.op = FilterList::Or;
        FilterField::Op matchFlag;
        if(userDbData->useRegExpSearch)
            matchFlag = FilterField::RegExp;
        else
            matchFlag = FilterField::Contains;
        searchText.fields.append(STableViewBase::initFilterField("t1.`id`", matchFlag, what));
        searchText.fields.append(STableViewBase::initFilterField("t1.`title`", matchFlag, what, Qt::CaseInsensitive));
        searchText.fields.append(STableViewBase::initFilterField("t1.`serial_number`", matchFlag, what, Qt::CaseInsensitive));
        searchText.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t5.`short_name`, t5.`ur_name`)", matchFlag, what, Qt::CaseInsensitive));
        searchText.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t5.`surname`, t5.`name`, t5.`patronymic`)", matchFlag, what, Qt::CaseInsensitive));
        searchText.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t6.`phone`, t6.`phone_clean`)", matchFlag, what));
        searchText.fields.append(STableViewBase::initFilterField("`ext_notes`", matchFlag, what));
        list.childs.append(searchText);
    }

    query_group.clear();
    query_group << "`id`";    // default GROUP part of query

    static_cast<STableRepairsModel*>(ui->tableView->model())->setFetchSize(displayOutOverride?40:300);
    ui->tableView->setFilter(list);
    ui->tableView->setGrouping(query_group);
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
    constructQueryClause();

    ui->tableView->refresh(preserveScrollPos, preserveSelection);
    if(!preserveSelection)  // при обновлении модели сигнал QItemSelectionModel::selectionChanged не генерируется
        updateWidgets();

    if(userDbData->autoRefreshWorkspace)
        ui->tableView->enableAutorefresh(userDbData->refreshTime*1000);
}

void tabRepairs::setFocusSearchField()
{
    ui->lineEditSearch->setFocus(Qt::MouseFocusReason);
    ui->lineEditSearch->selectAll();
}

void tabRepairs::tableItemDoubleClick(QModelIndex item)
{
    int id = repairs_table->unformattedData(item.siblingAtColumn(STableRepairsModel::Columns::Id)).toInt();
    if(!id)
        return;

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

void tabRepairs::tableItemClick(QModelIndex index)
{
    if(index.column() == STableViewRepairs::Column::ClientFullName && QGuiApplication::queryKeyboardModifiers() & Qt::AltModifier)
    {
        QString searchStr = ui->tableView->model()->data(index).toString();
        if(userDbData->useRegExpSearch)
        { // TODO: добавить все спец. символы RegExp
            searchStr = searchStr.replace("(", "\\(").replace(")", "\\)");
            searchStr = searchStr.replace("[", "\\[").replace("]", "\\]");
            searchStr = searchStr.replace("^", "\\^").replace("$", "\\$");
            searchStr = searchStr.replace("?", "\\?").replace("\\", "\\\\");
        }
        ui->lineEditSearch->setText(searchStr.replace("\\\\","\\"));
    }
}

/*  Поиск по введённому тексту
 *  Производится с задержкой после ввода для уменьшения кол-ва запросов и исключения тормозов интерфейса
*/
void tabRepairs::lineEditSearchTextChanged(QString)
{
    ui->tableView->clearSelection();
    ui->tableView->resetVScrollPos();
    constructQueryClause();
    ui->tableView->delayedRefresh(350);
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

void tabRepairs::buttonPrintClicked()
{
    ui->tableView->enableAutorefresh(0);
    tabPrintDialog *tab = tabPrintDialog::create(ui->tableView->mode()?(Global::Reports::cartridges):(Global::Reports::repairs));
    if(userDbData->autoRefreshWorkspace)
    {
#if QT_VERSION >= 0x060000
        connect(tab, &tabPrintDialog::renderFinished, this, [=]{
            ui->tableView->enableAutorefresh(userDbData->refreshTime*1000);
        }, Qt::SingleShotConnection);
#else
        auto con = QSharedPointer<QMetaObject::Connection>::create();
        *con = QObject::connect(tab, &tabPrintDialog::renderFinished, this, [con, this]{
            QObject::disconnect(*con);
            ui->tableView->enableAutorefresh(userDbData->refreshTime*1000);
        });
#endif
    }
    tab->addDataModel(repairs_table);
    tab->startRender();
}

void tabRepairs::tableSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    updateWidgets();
}

QList<SRepairModel*> tabRepairs::repairsListFromSelection()
{
    QList<SRepairModel*> list;
    QList<int> *idsList = ui->tableView->selectedRepairsList();
    SRepairModel *repair;
    SSaleTableModel *BOQModel;

    for(int i = 0; i < idsList->count(); i++)
    {
        try
        {
            repair = new SRepairModel();
            repair->load(idsList->at(i));
            BOQModel = new SSaleTableModel(repair);
            BOQModel->repair_loadTable(idsList->at(i));
            repair->setBOQModel(BOQModel);
            list.append(repair);
        }
        catch (Global::ThrowType)
        {
            delete repair;
        }
    }

    delete idsList;

    return list;
}

bool tabRepairs::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride)
    {
        // TODO: настраиваемые горячие клавиши
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier))
            switch(keyEvent->key())
            {
                // вызов метода accept() останавливает распространение события на родительские объекты (MainWindow не получит ShortcutOverride),
                // однако, следующие за ним события keyPress и keyRelease распространяются; TODO: разобраться нужно ли их фильтровать тоже
                case Qt::Key_F: setFocusSearchField(); event->accept(); break;
                case Qt::Key_N: buttonReceptClicked(); event->accept(); break;
                case Qt::Key_M: ui->switchTableMode->toggle(); event->accept(); break;
                case Qt::Key_P: buttonPrintClicked(); break;
                default: ;
            }
    }
    return tabCommon::event(event);
}

void tabRepairs::createDialogIssue()
{
    QList<SRepairModel*> list = repairsListFromSelection();
    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    m_dialogIssue->setListOwner(true);
    if(ui->switchTableMode->isChecked() == STableViewRepairs::ModeRepairs)
        connect(m_dialogIssue, &SDialogIssueRepair::printWorksLists, [=](){tabPrintDialog::printRepairWorksReports(list, true);});
    else
        connect(m_dialogIssue, &SDialogIssueRepair::printWorksLists, [=](){tabPrintDialog::printCartridgeWorksReports(list, true);});
}

void tabRepairs::menuRequest(QPoint pos)
{

    if(ui->tableView->mode() == STableViewRepairs::ModeCartridges)
    {
        int receiptPrintable = 1;
        int issuePrintable = 0;
        int state = repairs_table->unformattedData(ui->tableView->selectionModel()->currentIndex().siblingAtColumn(STableViewRepairs::Column::State)).toInt();

        // печать квитанции в любом статусе, кроме выданных; печать акта только для выданных
        switch (state)
        {
            case Global::RepStateIds::Returned:
            case Global::RepStateIds::ReturnedInCredit: receiptPrintable = 0; issuePrintable = 1; break;
            case Global::RepStateIds::ReturnedNoRepair: receiptPrintable = 0; break;    // TODO: для выданных без ремонта акт нужен?
        }

        const QList<QAction*> actions = tableCartridgesMenu->actions();
        for(QAction *action : actions)
        {
            switch (action->property("type").toInt())
            {
            case MenuCartridgesActions::PrintReceipt: action->setVisible(receiptPrintable); break;
            case MenuCartridgesActions::PrintIssue: action->setVisible(issuePrintable); break;
            }
        }
        tableCartridgesMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

/* Слот, вызываемый из контекстного меню */
void tabRepairs::printCartridgeReceiptReports()
{
    printCartridgeWorksReports();
}

/* Слот, вызываемый из контекстного меню */
void tabRepairs::printCartridgeWorksReports()
{
    tabPrintDialog::printCartridgeWorksReports(repairsListFromSelection(), true);
}


