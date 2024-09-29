#include "modules/purchasemanager/tabmanager.h"
#include "ui_tabmanager.h"
#include "tabprintdialog.h"

tabPurchaseManager* tabPurchaseManager::p_instance = nullptr;

tabPurchaseManager *tabPurchaseManager::getInstance(MainWindow *parent)
{
    if( !p_instance )
      p_instance = new tabPurchaseManager(parent);
    return p_instance;
}

tabPurchaseManager::tabPurchaseManager(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabPurchaseManager)
{
    logUserActivity();
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    QStringList excl;
    excl << QString::number(SPartRequest::State::Finished) << QString::number(SPartRequest::State::Cancelled);
    m_excludeStates = excl.join(',');
    // В АСЦ запросы на закупку не детерминированы по организации и офису
//    ui->comboBoxCompany->setModel(companiesModel);
//    ui->comboBoxOffice->setModel(officesModel);
//    ui->comboBoxOffice->setButtons("Clear");
    ui->comboBoxCreator->setModel(usersModel);
    ui->comboBoxCreator->setButtons("Clear");
    ui->comboBoxCreator->setPlaceholderText(tr("Все"));
    ui->comboBoxCreator->setCurrentIndex(-1);

    // TODO: подумать над дополнительным ограничением пользователю смотреть все заявки
    // (в АСЦ пользователь может просматривать только заявки по которым он назначен ответственным).
    ui->comboBoxManager->setModel(usersModel);
    ui->comboBoxManager->setButtons("Clear");
    ui->comboBoxManager->setPlaceholderText(tr("Все"));
    ui->comboBoxManager->setCurrentIndex(usersModel->rowByDatabaseID(userDbData->id));
    ui->labelManager->setVisible(permissions->viewAllPartsRequests);
    ui->comboBoxManager->setVisible(permissions->viewAllPartsRequests);

    ui->comboBoxState->setModel(partRequestStatesListModel);
    ui->comboBoxState->hideRow(partRequestStatesListModel->rowByDatabaseID(SPartRequest::NewUncommitted));
    ui->comboBoxState->setButtons("Clear");
    ui->comboBoxState->setPlaceholderText(tr("Все"));
    ui->comboBoxState->setCurrentIndex(-1);

    ui->comboBoxPriority->setModel(partRequestPrioritiesListModel);
    ui->comboBoxPriority->setButtons("Clear");
    ui->comboBoxPriority->setPlaceholderText(tr("Все"));
    ui->comboBoxPriority->setCurrentIndex(-1);

//    connect(ui->comboBoxOffice, qOverload<int>(&SComboBox::currentIndexChanged),
//            this, [=](int index){if(comboBoxIndexChanged(ui->comboBoxOffice, m_office, index)) filterChanged(index);});
    connect(ui->comboBoxCreator, qOverload<int>(&SComboBox::currentIndexChanged),
            this, [=](int index){if(comboBoxIndexChanged(ui->comboBoxCreator, m_creator, index)) filterChanged(index);});
    if(permissions->viewAllPartsRequests)
    {
        connect(ui->comboBoxManager, qOverload<int>(&SComboBox::currentIndexChanged),
                this, [=](int index){if(comboBoxIndexChanged(ui->comboBoxManager, m_manager, index)) filterChanged(index);});
    }
    connect(ui->comboBoxState, qOverload<int>(&SComboBox::currentIndexChanged),
            this, [=](int index){if(comboBoxIndexChanged(ui->comboBoxState, m_state, index)) filterChanged(index);});
    connect(ui->comboBoxPriority, qOverload<int>(&SComboBox::currentIndexChanged),
            this, [=](int index){if(comboBoxIndexChanged(ui->comboBoxPriority, m_priority, index)) filterChanged(index);});

    tableUpdateDelay = new QTimer(this);
    connect(tableUpdateDelay, &QTimer::timeout, this, &tabPurchaseManager::refreshManual);
    tableUpdateDelay->setSingleShot(true);

    groupsModel = SPartsRequestsGroupingModel::groupsList();
    ui->comboBoxGrouping->setModel(groupsModel);

    subgroupsModel = new SPartsRequestsGroupingModel();
    ui->tableViewSubgroups->setModel(subgroupsModel);
    ui->widgetPartSuppliers->setSubgroupsModel(subgroupsModel);
    connect(ui->comboBoxGrouping, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &tabPurchaseManager::groupChanged);
    connect(ui->tableViewSubgroups->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, [=](const QModelIndex &current, const QModelIndex&){subgroupIndexChanged(current);});
    connect(ui->tableViewSubgroups, &STableViewSubgroups::refreshed, this, &tabPurchaseManager::subgroupsUpdated);
    connect(ui->widgetPartsRequests, &SPartsRequests::rowSelected, ui->widgetPartSuppliers, &SPartSuppliers::select);
    connect(ui->widgetPartsRequests, &SPartsRequests::requestStateChanged, ui->widgetPartSuppliers, &SPartSuppliers::setRequestState);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &tabPurchaseManager::lineEditSearchTextChanged);

    ui->widgetPeriodSelector->setMovingInterval(SPeriodSelector::Year);
    ui->widgetPeriodSelector->setDefaultPeriod();
    connect(ui->widgetPeriodSelector, &SPeriodSelector::refreshButtonClicked, this, &tabPurchaseManager::refreshManual);
    connect(ui->pushButtonCreateNew, &QPushButton::clicked, this, []{MainWindow::getInstance()->createTabPartRequest(0);});
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &tabPurchaseManager::refreshManual);
    connect(ui->widgetPartSuppliers, &SPartSuppliers::beginRowInsert, this, [=]{commitChanges(checkChanges(DirtyFlags::SuppliersModel));});
    connect(ui->widgetPartSuppliers, &SPartSuppliers::submitOk, this, &tabPurchaseManager::linksSaveOk);

    groupChanged(SPartsRequestsGroupingModel::Group::No);

#ifdef QT_DEBUG
//    ui->lineEditSearch->setText("black, 40mm");
    ui->widgetPeriodSelector->setPeriodBegin(QDate(2023,1,1));
    ui->comboBoxCreator->setCurrentIndex(usersModel->rowByDatabaseID(33));
    ui->comboBoxGrouping->setCurrentIndex(groupsModel->rowByDatabaseID(SPartsRequestsGroupingModel::Supplier));
//    ui->comboBoxPriority->setCurrentIndex(partRequestPrioritiesListModel->rowByDatabaseID(SPartRequest::Priority::Low));
#endif
}

tabPurchaseManager::~tabPurchaseManager()
{
    delete ui;
    p_instance = nullptr;
}

QString tabPurchaseManager::tabTitle()
{
    return tr("Менеджер закупок");
}

void tabPurchaseManager::groupChanged(int index)
{
    if(!comboBoxIndexChanged(ui->comboBoxGrouping, m_grouping, index))
        return;

    MAINCONN_QUERY_LOG_START(metaObject()->className());

    QString query;

    switch (index)
    {
        case SPartsRequestsGroupingModel::Group::No:       query = constructSubgroupListQuery(index); break;
        case SPartsRequestsGroupingModel::Group::Repair:   query = constructSubgroupListQuery(index, "repair"); break;
        case SPartsRequestsGroupingModel::Group::Client:   query = constructSubgroupListQuery(index, "client"); break;
        case SPartsRequestsGroupingModel::Group::Tracking: query = constructSubgroupListQuery(index, "tracking"); break;
        case SPartsRequestsGroupingModel::Group::Supplier: query = constructSubgroupListQuery(index); break;
        default: break;
    }

    ui->widgetPartSuppliers->setGroupingMode(index);
    ui->tableViewSubgroups->setQuery(query, QSqlDatabase::database("connMain"));
    ui->tableViewSubgroups->refresh();

    MAINCONN_QUERY_LOG_STOP;
}

void tabPurchaseManager::constructQueryClause(FilterList &filter, const int &group)
{
    int index;

    filter.fields.append(STableViewBase::initFilterField("t1.`request_time`", FilterField::More, ui->widgetPeriodSelector->periodBegin()));
    filter.fields.append(STableViewBase::initFilterField("t1.`request_time`", FilterField::LessEq, ui->widgetPeriodSelector->periodEnd()));

    // В АСЦ запросы на закупку не детерминируются по организации и офису
//    index = ui->comboBoxCompany->currentIndex();
//    if (index >= 0)
//        filter.fields.append(STableViewBase::initFilterField("t1.`company`", FilterField::Equals, companiesModel->databaseIDByRow(index)));

//    index = ui->comboBoxOffice->currentIndex();
//    if(index >= 0)
//        filter.fields.append(STableViewBase::initFilterField("t1.`office`", FilterField::Equals, officesModel->databaseIDByRow(index)));

    index = ui->comboBoxCreator->currentIndex();
    if(index >= 0)
        filter.fields.append(STableViewBase::initFilterField("t1.`from_user`", FilterField::Equals, usersModel->databaseIDByRow(index)));

    index = ui->comboBoxManager->currentIndex();
    if(index >= 0)
        filter.fields.append(STableViewBase::initFilterField("t2.`emploee`", FilterField::Equals, usersModel->databaseIDByRow(index)));

    // Если фильтр по статусу не выбран, то завершенные и отменённые не учитываются
    index = ui->comboBoxState->currentIndex();
    if(index >= 0)
        filter.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::Equals, partRequestStatesListModel->databaseIDByRow(index)));
    else
        filter.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::NotMark|FilterField::InSet, m_excludeStates));

    index = ui->comboBoxPriority->currentIndex();
    if(index >= 0)
        filter.fields.append(STableViewBase::initFilterField("t1.`pririty`", FilterField::Equals, partRequestPrioritiesListModel->databaseIDByRow(index)));

    if(!ui->lineEditSearch->text().isEmpty())
    {
        FilterList search;
        search.op = FilterList::Or;
        FilterField::Op matchFlag;
        if(userDbData->useRegExpSearch)
            matchFlag = FilterField::RegExp;
        else
            matchFlag = FilterField::Contains;

        search.fields.append(STableViewBase::initFilterField("t1.`item_name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));

        // Вызов метода без передачи параметра group предназначен для формирования условий запроса для модели данных SPartsRequestsModel;
        switch(group)
        {
            case -1:
            case SPartsRequestsGroupingModel::Group::Client:
            case SPartsRequestsGroupingModel::Group::Supplier: ; break;
            default: filter.childs.append(search); return;
        }

        if(group != SPartsRequestsGroupingModel::Group::Supplier)
        {
            FilterList clientName;
            clientName.op = FilterList::Or;
            clientName.fields.append(STableViewBase::initFilterField("t3.`name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            clientName.fields.append(STableViewBase::initFilterField("t3.`surname`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            clientName.fields.append(STableViewBase::initFilterField("t3.`ur_name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            search.childs.append(clientName);
        }

        if(group != SPartsRequestsGroupingModel::Group::Client)
        {
            FilterList url;
            url.op = FilterList::Or;
            search.fields.append(STableViewBase::initFilterField("t4.`supplier_url`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            search.fields.append(STableViewBase::initFilterField("t4.`item_url`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            search.childs.append(url);

            FilterList supplierName;
            supplierName.op = FilterList::Or;
            supplierName.fields.append(STableViewBase::initFilterField("t5.`name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            supplierName.fields.append(STableViewBase::initFilterField("t5.`surname`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            supplierName.fields.append(STableViewBase::initFilterField("t5.`ur_name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
            search.childs.append(supplierName);
        }

        filter.childs.append(search);
    }
}

QString tabPurchaseManager::constructSubgroupListQuery(const int &group, const QString &field)
{
    QString query;
    QString clause;
    FilterList filter;

    filter.op = FilterList::And;


    switch (group)
    {
        case SPartsRequestsGroupingModel::Group::Client: query = QUERY_SEL_PARTS_REQUESTS_GROUP_CLIENT; break;
        case SPartsRequestsGroupingModel::Group::Supplier: query = QUERY_SEL_PARTS_REQUESTS_GROUPS_SUPPLIER; break;
        case SPartsRequestsGroupingModel::Group::Repair:
        case SPartsRequestsGroupingModel::Group::Tracking: query = QUERY_SEL_PARTS_REQUESTS_GROUP_FIELD(field); break;
        case SPartsRequestsGroupingModel::Group::No:
        default: return QUERY_SEL_PARTS_REQUESTS_NO_GROUPING;
    }

    constructQueryClause(filter, group);

    clause = ui->tableViewSubgroups->formatFilterGroup(filter);

    if(!clause.isEmpty())
        query = query.replace("GROUP BY", "  AND " + clause + "\r\nGROUP BY");

    return query;
}

void tabPurchaseManager::constructMainTableQueryFilter()
{
    int group = ui->comboBoxGrouping->currentIndex();
    const QModelIndex &subgroup = ui->tableViewSubgroups->currentIndex();
    int id = -1;
    QVariant value;
    FilterList filter;

    filter.op = FilterList::And;

    constructQueryClause(filter);

    if(group == SPartsRequestsGroupingModel::Group::No || subgroup.siblingAtColumn(SPartsRequestsGroupingModel::Columns::C).data().toInt() == 0)
    {
        ui->widgetPartsRequests->setFilter(filter);
        return;
    }

    id = subgroupsModel->id(subgroup);
    value = id;
    if(id == -1)
    {
        value = subgroupsModel->name(subgroup);
    }

    switch (group)
    {
        case SPartsRequestsGroupingModel::Group::Repair:   filter.fields.append(STableViewBase::initFilterField("t1.`repair`", FilterField::Equals, value)); break;
        case SPartsRequestsGroupingModel::Group::Client:   filter.fields.append(STableViewBase::initFilterField("t1.`client`", FilterField::Equals, value)); break;
        case SPartsRequestsGroupingModel::Group::Tracking: filter.fields.append(STableViewBase::initFilterField("t1.`tracking`", FilterField::Equals, value, Qt::CaseInsensitive)); break;
        case SPartsRequestsGroupingModel::Group::Supplier: {
            if(id == -1)
                filter.fields.append(STableViewBase::initFilterField("t4.`supplier_url`", FilterField::Equals, value, Qt::CaseInsensitive));
            else
            {
                FilterList supplId;
                supplId.op = FilterList::Or;
                supplId.fields.append(STableViewBase::initFilterField("t1.`dealer`", FilterField::Equals, value));
                supplId.fields.append(STableViewBase::initFilterField("t4.`supplier_id`", FilterField::Equals, value));
                filter.childs.append(supplId);
            }

            break; }
        default: STableViewBase::initFilterField(QString(), FilterField::NoOp, 0); break;
    }

    ui->widgetPartsRequests->setFilter(filter);
}

void tabPurchaseManager::filterChanged(int)
{
    groupChanged(ui->comboBoxGrouping->currentIndex());
}

int tabPurchaseManager::checkChanges(int mask)
{
    if(!mask)
        return 0;

    return ((ui->widgetPartsRequests->isDirty() << 1) | (ui->widgetPartSuppliers->isDirty() << 0)) & mask;
}

/* Вызов метода проверки несохранённых данных в таблицах заявок и ссылок
 * result бдует содержать ответ пользователя
 * с помощью mask можно задать вариаты проверки (битовые флаги)
 * с помощью buttons можно переопределить кнопки диалогового окна; по умолчанию: Да, Нет и Отмена
 */
void tabPurchaseManager::confirmChanges(int &result, int mask, QMessageBox::StandardButtons buttons)
{
    int flags = 0;
    flags = checkChanges(mask);

    if(flags)
    {
        result = QMessageBox::question(this,
                                       tr("Данные не сохранены"),
                                       tr("Заявки были отредактированы!\nСохранить?"),
                                       buttons);
    }

    if(result == QMessageBox::Yes)
    {
        if(!commitChanges(flags))
            result = QMessageBox::Cancel;
    }
    // Обработка нажатий No и Cancel происходит в вызывающем методе
}

bool tabPurchaseManager::commitChanges(int flags)
{
    if(!flags)
        return 1;

    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        if(flags & DirtyFlags::SuppliersModel)
            ui->widgetPartSuppliers->saveLinks();
        if(flags & DirtyFlags::RequestsModel)
            ui->widgetPartsRequests->commit();
        QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    catch(Global::ThrowType type)
    {
        nErr = 0;
        if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    QUERY_LOG_STOP;

    delete query;

    return nErr;
}

void tabPurchaseManager::subgroupsUpdated()
{
    bool widgetState = 1;

    if(subgroupsModel->rowCount() == 1)
    {
        if(subgroupsModel->index(0, SPartsRequestsGroupingModel::Columns::C).data().toInt() == 0)
        {
            widgetState = 0;
        }
    }

    ui->tableViewSubgroups->setEnabled(widgetState);
}

void tabPurchaseManager::subgroupIndexChanged(const QModelIndex &item)
{
    if(!item.isValid())
        return;

    ui->widgetPartSuppliers->clearModel();
    constructMainTableQueryFilter();
    ui->widgetPartsRequests->refresh(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

bool tabPurchaseManager::comboBoxIndexChanged(QComboBox *widget, int &oldIndex, int newIndex)
{
    int result = 0;
    if(oldIndex != newIndex)
        confirmChanges(result, 3);

    switch (result)
    {
        case QMessageBox::Cancel: undoComboBoxIndexChange(widget, oldIndex); return 0;
        case QMessageBox::No: break;
        default: ;  // Обработка нажатия Yes происходит в методе confirmChanges
    }

    oldIndex = newIndex;
    return 1;
}

void tabPurchaseManager::undoComboBoxIndexChange(QComboBox *widget, int &oldIndex)
{
    widget->blockSignals(true);
    widget->setCurrentIndex(oldIndex);
    widget->blockSignals(false);
}

void tabPurchaseManager::lineEditSearchTextChanged(QString)
{
    ui->tableViewSubgroups->setQuery(constructSubgroupListQuery(ui->comboBoxGrouping->currentIndex()), QSqlDatabase::database("connMain"));
    ui->tableViewSubgroups->delayedRefresh(350);
}

/* Обновление списков, вызываемое программно
 * Выполняется только при отсутствии несохранённых изменений.
*/
void tabPurchaseManager::refresh()
{
    if(checkChanges(3))
        return;

    refreshQuiet();
}

/* Обновление списков, вызываемое действием пользователя
 * Перед обновлением производится проверка наличия несохранённых
 * измененений и вывод диалога при их наличии
*/
void tabPurchaseManager::refreshManual()
{
    int result = 0;
    confirmChanges(result, 3);
    switch (result)
    {
        case QMessageBox::Cancel: return;
        case QMessageBox::No: break;
        default: ;  // Обработка нажатия Yes происходит в методе confirmChanges
    }

    ui->widgetPartSuppliers->clearModel();
    groupChanged(m_grouping);   // обновление списка подгрупп приведёт к обновлению списка заявок
}

/* Обновление списков подгруп, запросов и ссылок с сохранением текущего положения прокрутки и выделенных строк
*/
void tabPurchaseManager::refreshQuiet()
{
    ui->widgetPartsRequests->tableView()->blockSignals(true);
    ui->tableViewSubgroups->refresh(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
    ui->widgetPartsRequests->tableView()->refresh(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
    ui->widgetPartSuppliers->refresh(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
    ui->widgetPartsRequests->tableView()->blockSignals(false);
}

void tabPurchaseManager::linksSaveOk()
{
    shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Менеджер закупок"), tr("Изменения успешно сохранёны"), QColor(214,239,220), QColor(229,245,234));
    refreshQuiet();
}

bool tabPurchaseManager::tabCloseRequest()
{
    int result = 0;
    confirmChanges(result, 3);
    switch (result)
    {
        case QMessageBox::Cancel: return 0;
        case QMessageBox::No: break;
        default: ;  // Обработка нажатия Yes происходит в методе confirmChanges
    }

    return 1;
}

void tabPurchaseManager::refreshIfTabExists()
{
    tabPurchaseManager *tabInstance = nullptr;

    tabInstance = MainWindow::getInstance()->findChild<tabPurchaseManager*>();
    if(tabInstance)
    {
        tabInstance->refresh();
    }
}

STableViewSubgroups::STableViewSubgroups(QWidget *parent) :
    STableViewBase(SLocalSettings::BuyRequestSubgroupsGrid, parent)
{
    readLayout();

    setUniqueIdColumn(SPartsRequestsGroupingModel::Group);
    viewport()->installEventFilter(this);
}

STableViewSubgroups::~STableViewSubgroups()
{

}

bool STableViewSubgroups::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    if(object == viewport())
    {
        switch(event->type())
        {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseMove: return mouseEventOnCell(event);
            default: break;
        }
    }

    return false;
}

void STableViewSubgroups::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SPartsRequestsGroupingModel*>(model);
    STableViewBase::setModel(model);
}

void STableViewSubgroups::clearModel()
{
    if(!m_model)
        return;

    m_model->clear();
}

void STableViewSubgroups::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    if(!m_model)
        return;

    m_model->setQuery(query, database);
}

QString STableViewSubgroups::formatFilterGroup(const FilterList &filter)
{
    return STableViewBase::formatFilterGroup(filter);
}

bool STableViewSubgroups::mouseEventOnCell(QEvent *event)
{
    tabPurchaseManager *tab = dynamic_cast<tabPurchaseManager*>(this->parent()->parent());
    if(!tab)
        return false;

    // Если ни одна строка в таблице не выделена проверять изменения не нужно
    QModelIndexList selection = selectionList();
    if(selection.empty())
        return false;

    QMouseEvent *e = static_cast<QMouseEvent*>(event);
    QModelIndex index = indexAt(e->localPos().toPoint());

    // При щелчке по другой ячейке в той же (выделенной) строке проверять изменения не нужно
    if(selection.first().row() == index.row())
        return false;

    int result = 0;
    tab->confirmChanges(result, 3);

    switch (result)
    {
        case QMessageBox::Cancel: return true;
        case QMessageBox::No: return false;
        default: ;
    }

    return false;
}

void STableViewSubgroups::refreshPending()
{
    int result = 0;
    tabPurchaseManager *tab = dynamic_cast<tabPurchaseManager*>(this->parent()->parent());
    if(!tab)
        return;

    tab->confirmChanges(result, 3);
    if(result != QMessageBox::Cancel)
    {
        refresh(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
        QItemSelection selection;
        QItemSelectionRange rowSel(model()->index(0, 0), model()->index(0, model()->columnCount() - 1));
        selection.append(rowSel);
        selectionModel()->select(selection, QItemSelectionModel::Select);
        tab->subgroupIndexChanged(model()->index(0, 0));
    }
}

void STableViewSubgroups::setDefaultLayoutParams()
{
    STableViewBase::setDefaultLayoutParams();
    i_gridLayout->$GridControl.Columns[Column::Group].Width_marked = true;  // автоширина по умолчанию
    i_gridLayout->$GridControl.Columns[Column::Selected].ActualWidth = 0;
    i_gridLayout->$GridControl.Columns[Column::Selected].Visible = false;
}

/* Активация первого элемента в списке
 * Метод должен вызываться при обновлении списка подгрупп:
 *  - нажатием кнопки "Обновить";
 *  - применением фильтра по какому либо критерию;
*/
void STableViewSubgroups::resetSubgroupIndex()
{
    if(model()->rowCount() >= 1)
    {
//        QModelIndexList match;
        QModelIndex prev = currentIndex();
        QModelIndex all = model()->index(0, 0);

        // поиск элемента "Все" и его активация (может быть неудобно если подгрупп много)
//        match = model()->match(model()->index(0, SPartsRequestsGroupingModel::Columns::C), Qt::DisplayRole, 0);
//        if(match.count())
//            all = match.first();

        setCurrentIndex(all);
        emit selectionModel()->currentRowChanged(all, prev);
    }
}

void STableViewSubgroups::refresh(bool preserveScrollPos, bool preserveSelection)
{
    STableViewBase::refresh(preserveScrollPos, preserveSelection);
    emit refreshed();
    if(preserveScrollPos == ScrollPosReset && preserveSelection == SelectionReset)
        resetSubgroupIndex();
}

void STableViewSubgroups::translateNames()
{
    tr("Group");
    tr("Total");
}
