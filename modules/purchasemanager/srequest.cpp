#include "srequest.h"
#include "widgets/snotification.h"

SPartRequest::SPartRequest(QObject *parent)
    : SComRecord{parent}
{
    i_obligatoryFields << "request_time" << "from_user" << "state" << "item_name" << "notes" << "url" << "pririty";
    i_tableName = "parts_request";
    i_idColumnName = "id";

    i_createdUtc = QDateTime::currentDateTimeUtc();
    i_logRecord->setType(SLogRecordModel::RecordTypes::PartRequest);

    m_managers = new SEditableBaseModel(this, QSqlDatabase::database("connThird"));
    m_managers->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_managers->setTable("parts_request_employees");

    m_notifications = new SEditableBaseModel(this, QSqlDatabase::database("connThird"));
    m_notifications->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_notifications->setTable("notifications");

    connect(m_managers, &SEditableBaseModel::primeInsert, this, &SPartRequest::managersPrimeInsert);
    connect(m_notifications, &SEditableBaseModel::primeInsert, this, &SPartRequest::notificationsPrimeInsert);
}

SPartRequest::~SPartRequest()
{

}

SStandardItemModel *SPartRequest::statesList()
{
    SStandardItemModel *list;
    QMetaEnum types = SPartRequest::staticMetaObject.enumerator(SPartRequest::staticMetaObject.indexOfEnumerator("State"));

    list = SStandardItemModel::modelFromEnum(types, tr);
    // Статус "Новая" не может быть выбран пользователем (программно можно)
    QStandardItem* item = list->item(list->rowByDatabaseID(SPartRequest::NewUncommitted));
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    list->setObjectName("partRequestStatesList");

    return list;
}

SStandardItemModel *SPartRequest::prioritiesList()
{
    SStandardItemModel *list;
    QMetaEnum types = SPartRequest::staticMetaObject.enumerator(SPartRequest::staticMetaObject.indexOfEnumerator("Priority"));

    list = SStandardItemModel::modelFromEnum(types, tr);
    list->setObjectName("partRequestPrioritiesList");

    return list;

}

/* Статический метод инициализации модели менеджеров заявок на закупку.
 * Должен вызываться только при запуске приложения.
*/
SSqlQueryModel *SPartRequest::managersList()
{
    SSqlQueryModel *model = new SSqlQueryModel;
    model->setQuery(QUERY_SEL_PRT_RQST_MANAGERS(permissions->permissionId("enableSparepartRequestManager")), QSqlDatabase::database("connMain"));
    model->setObjectName("partRequestManagersModel");

    return model;
}

int SPartRequest::id()
{
    return i_id;
}

void SPartRequest::setId(const int id)
{
    i_id = id;
}

void SPartRequest::load(const int &id, QSqlDatabase db)
{
    QSqlDatabase database = db;
    if(!database.isValid())
        database = QSqlDatabase::database("connMain");

    QSqlQuery *record = new QSqlQuery(database);

    beginResetModel();
    i_valuesMap.clear();

    record->exec(QUERY_SEL_PART_REQUEST(id));

    if(record->first())
    {
        i_id = record->value("id").toInt();
        i_createdUtc = record->value("request_time").toDateTime();
        m_employee = record->value("from_user").toInt();
        m_repair = record->value("repair").toInt();
        m_client = record->value("client").toInt();
        m_state = record->value("state").toInt();
        m_endDate = record->value("end_date").toDate();
        m_planEndDate = record->value("plan_end_date").toDate();
        m_amount = record->value("summ").toDouble();
        m_tracking = record->value("tracking").toString();
        m_itemId = record->value("item_id").toInt();
        m_name = record->value("item_name").toString();
        m_notes = record->value("notes").toString();
        m_url = record->value("url").toString();
        m_priority = record->value("pririty").toInt();
        m_count = record->value("count").toInt();
        m_dealer = record->value("dealer").toInt();

        updateAdditionalModelsFilters(i_id);
        m_managers->select();
        m_notifications->select();
    }

    delete record;
    endResetModel();
}

QDateTime SPartRequest::createdUtc()
{
    return cachedValue("request_time", i_createdUtc).toDateTime();
}

/*  Установка даты создания
 *  TODO: подумать о необходимости возможности установки будущей даты для не срочных заявок
 *  timestamp - локальное время (не UTC)
 */
void SPartRequest::setCreated(const QDateTime &timestamp)
{
    if(m_state != NewUncommitted)
    {
        if(createdUtc() == timestamp.toUTC())
            return;
    }

    i_createdUtc = timestamp.toUTC();
    i_valuesMap.insert("request_time", timestamp);
}

int SPartRequest::employee()
{
    return cachedValue("from_user", m_employee).toInt();
}

void SPartRequest::setEmployee(const int &id)
{
    if(m_state != NewUncommitted)
        return;

    i_valuesMap.insert("from_user", id);
}

int SPartRequest::repair()
{
    return cachedValue("repair", m_repair).toInt();
}

void SPartRequest::setRepair(const int &id)
{
    if(this->repair() == id)
        return;

    i_valuesMap.insert("repair", id);
}

int SPartRequest::client()
{
    return cachedValue("client", m_client).toInt();
}

void SPartRequest::setClient(const int &id)
{
    if(this->client() == id)
        return;

    i_valuesMap.insert("client", id?id:QVariant());
}

int SPartRequest::state()
{
    return cachedValue("state", m_state).toInt();
}

void SPartRequest::setState(const int &newState)
{
    int currentState = this->state();
    if(currentState == newState)
        return;

    // сброс статуса в случае отката изменений при создании заявки (Created -» NewUncommitted)
    if(newState == NewUncommitted && currentState == Created)
    {
        i_valuesMap.remove("state");
        setCreated(QDateTime());
        return;
    }

    if(currentState == NewUncommitted)
    {
        //  (NewUncommitted -» Created)
        if(newState == Created && !createdUtc().isValid())
            setCreated(QDateTime::currentDateTime());
    }
    else    //  (Any -» Created)
        appendLogText(logMsgStateChange(newState));

    if(newState == Finished)
        setEndDate(QDate::currentDate());

    i_valuesMap.insert("state", newState);
}

QString SPartRequest::logMsgStateChange(const int &newState)
{
    return tr("Статус изменён на %1").arg(partRequestStatesListModel->getDisplayRole(newState));
}

QDate SPartRequest::endDate()
{
    return cachedValue("end_date", m_endDate).toDate();
}

void SPartRequest::setEndDate(const QDate &date)
{
    if(endDate() == date)
        return;

    i_valuesMap.insert("end_date", date);
}

QDate SPartRequest::planEndDate()
{
    return cachedValue("plan_end_date", m_planEndDate).toDate();
}

void SPartRequest::setPlanEndDate(const QDate &date)
{
    if(planEndDate() == date)
        return;

    // эти записи в журнал не производятся создании заявки, а только при изменении
    QString logText;
    if(m_state != NewUncommitted)
    {
        if(!planEndDate().isValid())
            logText = tr("Указана плановая дата завершения: %1").arg(date.toString("dd.MM.yyyy"));
        else
            logText = logMsgPlanEndDateChange(date);
    }

    i_valuesMap.insert("plan_end_date", date);
    appendLogText(logText);
}

QString SPartRequest::logMsgPlanEndDateChange(const QDate &date)
{
    return tr("Плановая дата завершения изменёна на %1").arg(date.toString("dd.MM.yyyy"));
}

double SPartRequest::amount()
{
    return cachedValue("summ", m_amount).toDouble();
}

void SPartRequest::setAmount(const double &amount)
{
    if(this->amount() == amount)
        return;

    QString logText;
    if(this->amount() == 0)
        logText = tr("Указана сумма: %1").arg(sysLocale.toCurrencyString(amount));
    else
        logText = logMsgAmountChange(amount);

    i_valuesMap.insert("summ", amount);
    appendLogText(logText);
}

QString SPartRequest::logMsgAmountChange(const double &amount)
{
    return tr("Сумма изменёна на %1").arg(sysLocale.toCurrencyString(amount));
}

QString SPartRequest::tracking()
{
    return cachedValue("tracking", m_tracking).toString();
}

void SPartRequest::setTracking(const QString &tracking)
{
    if(this->tracking() == tracking)
        return;

    QString logText;
    logText = logMsgTrackingChange(tracking, this->tracking());

    i_valuesMap.insert("tracking", tracking);
    appendLogText(logText);
}

QString SPartRequest::logMsgTrackingChange(const QString &newTrack, const QString &oldTrack)
{
    if(oldTrack.isEmpty())
        return tr("Указан трэк-номер: %1").arg(newTrack);

    if(newTrack.isEmpty())
        return tr("Трэк-номер удалён");

    return tr("Трэк-номер изменён на %1").arg(newTrack);
}

int SPartRequest::itemId()
{
    return cachedValue("item_id", m_itemId).toInt();
}

void SPartRequest::setItemId(const int &id)
{
    if(itemId() == id)
        return;

    i_valuesMap.insert("item_id", id);
}

QString SPartRequest::name()
{
    return cachedValue("item_name", m_name).toString();
}

void SPartRequest::setName(const QString &name)
{
    if(m_state != NewUncommitted && this->name() == name)
        return;

    i_valuesMap.insert("item_name", name);
}

QString SPartRequest::notes()
{
    return cachedValue("notes", m_notes).toString();
}

void SPartRequest::setNotes(const QString &notes)
{
    if(this->notes() == notes && m_state != NewUncommitted)
        return;

    i_valuesMap.insert("notes", notes);
}

QString SPartRequest::url()
{
    return cachedValue("url", m_url).toString();
}

void SPartRequest::setUrl(const QString &url)
{
    if(m_state != NewUncommitted && this->url() == url)
        return;

    i_valuesMap.insert("url", url);
}

int SPartRequest::priority()
{
    return cachedValue("pririty", m_priority).toInt();
}

void SPartRequest::setPriority(const int &priority)
{
    if(m_state != NewUncommitted)
    {
        if(this->priority() == priority)
            return;

        appendLogText(logMsgPriorityChange(priority));
    }

    i_valuesMap.insert("pririty", priority);
}

QString SPartRequest::logMsgPriorityChange(const int &priority)
{
    return tr("Приоритет изменён на %1").arg(partRequestPrioritiesListModel->getDisplayRole(priority));
}

int SPartRequest::count()
{
    return cachedValue("count", m_count).toInt();
}

void SPartRequest::setCount(const int &count)
{
    if(m_state != NewUncommitted)
    {
        if(this->count() == count)
            return;

        appendLogText(logMsgCountChange(count));
    }

    i_valuesMap.insert("count", count);
}

QString SPartRequest::logMsgCountChange(const int &count)
{
    return tr("Количество изменёно на %1").arg(count);
}

int SPartRequest::dealer()
{
    return cachedValue("dealer", m_dealer).toInt();
}

void SPartRequest::setDealer(const int &id)
{
    if(dealer() == id)
        return;

    i_valuesMap.insert("dealer", id?id:QVariant());
}

/* Общий метод для синхронизации данных моделей таблиц notifications и parts_request_employees
 * с переданным списком Id
*/
void SPartRequest::updateAdditionalModel(SEditableBaseModel *model, QList<int> newList, const int columnUser)
{
    QList<int> current;

    for(int i = model->rowCount(); i > 0; --i)
    {
        int id = model->index(i - 1, columnUser).data().toInt();
        if(!newList.contains(id))
        {
            model->removeRow(i - 1);
            continue;
        }

        current.append(id);
    }

    if(current.count() == newList.count())
        return;

    for (int id : newList)
    {
        if(!current.contains(id))
        {
            int row = model->rowCount();
            model->insertRow(row);
            model->setData(model->index(row, columnUser), id);
        }
    }
}

void SPartRequest::updateAdditionalModelsFilters(const int id)
{
    QString filter;

    filter = QString("`request` = %1").arg(i_id);
    if(filter.compare(m_managers->filter()))
        m_managers->setFilter(filter);

    filter = QString("`part_request_id` = %1").arg(i_id);
    if(filter.compare(m_notifications->filter()))
        m_notifications->setFilter(filter);
}

QList<int> SPartRequest::managers()
{
    QList<int> list;

    if(m_managers->rowCount() == 0)
    {
        for(int i = 0; i < partRequestManagersModel->rowCount(); i++)
            list.append(partRequestManagersModel->databaseIDByRow(i));
    }

    for(int i = 0; i < m_managers->rowCount(); i++)
    {
        list.append(m_managers->index(i, ManagersModelFields::Employee).data().toInt());
    }

    return list;
}

void SPartRequest::setManagers(QList<int> managers)
{
    Q_ASSERT_X(i_id, "SPartRequest::setManagers()", "request id not set; porbably this method invoked before method commit()");
    updateAdditionalModel(m_managers, managers, ManagersModelFields::Employee);
    if(comSettings->notifyItemPurchaseRequest)
        updateNotifications(managers);
}

/* Метод служит для создания или удаления уведомлений пользователям о заявке на закупку.
 * Внутри метода происходит сравнение списка отмеченных галочкой пользовтелей с данными
 * модели и синхронизация; в случае ошибки при создании новой заявки, например, выбрал
 * не того менеджера, старое уведомление будет удалено и создано новое.
*/
void SPartRequest::updateNotifications(QList<int> managers)
{
    QList<int> list = managers;
    list.removeOne(userDbData->id); // уведомление самому себе не нужно
    list.removeOne(employee()); // уведомление инициатору не нужно
    updateAdditionalModel(m_notifications, list, NotificationsModelFields::Employee);
}

void SPartRequest::setDirty(const bool state)
{
    SDatabaseRecord::setDirty(state);
}

bool SPartRequest::commit()
{
    if(i_id)
    {
    }
    else
    {
        i_logTexts->clear();

        QStringList logs;
        logs.append(tr("Заявка на закупку %1ед. \"%2\" создана").arg(count()).arg(name()));
        logs.append(tr("Приоритет: %1").arg(partRequestPrioritiesListModel->getDisplayRole(priority())));
        if(planEndDate().isValid())
            logs.append(tr("Deadline: %1").arg(planEndDate().toString(sysLocale.dateFormat(QLocale::ShortFormat))));
        appendLogText(logs.join(". "));

        setUrl("");
    }

    return SComRecord::commit();
}

bool SPartRequest::isManagersModelDirty()
{
    return m_managers->isDirty();
}

bool SPartRequest::commitManagers()
{
    SDatabaseRecord::checkSystemTime();

    if(!m_managers->submitAll())
        Global::throwError(m_managers->lastError(), tr("Не удалось сохранить ответственных сотрудников в заявке на закупку"));
    if(!m_notifications->submitAll())
        Global::throwError(m_notifications->lastError(), tr("Не удалось отправить уведомления ответственным сотрудникам при сохранении заявки на закупку"));

    return i_nErr;
}

/* Переопределённый метод
 * Установка ID запроса на закупку в поле `logs`.`part_request`
*/
void SPartRequest::updateLogAssociatedRecId()
{
    i_logRecord->setPartRequestId(i_id);
}

/* Переопределённый метод
 * TODO: Это временное решение; метод будет удалён после переработки всех наследующих
 * классов SComRecord (см. более детальное описание там); в данном случае очистка кэша не
 * нужна, т. к. в этом классе геттеры возвращают актуальное значение: если изменялось, то из
 * кэша, в противном случае из приватной переменной, проинициализированной в методе load();
*/
void SPartRequest::dbErrFlagHandler(bool flushCache)
{
    Q_UNUSED(flushCache);

    SComRecord::dbErrFlagHandler(false);
}

void SPartRequest::managersPrimeInsert(int row, QSqlRecord &record)
{
    Q_UNUSED(row)

    m_managers->setRecordValue(record, ManagersModelFields::RequestId, i_id);
}

void SPartRequest::notificationsPrimeInsert(int row, QSqlRecord &record)
{
    Q_UNUSED(row)

    int type = SNotification::Type::BuyRequest;
    int client = this->client();
    int repair = this->repair();

    m_notifications->setRecordValue(record, NotificationsModelFields::Created, QDateTime::currentDateTimeUtc());
    m_notifications->setRecordValue(record, NotificationsModelFields::Employee, m_employee);
    m_notifications->setRecordValue(record, NotificationsModelFields::Type, type);
    if(i_id)
        m_notifications->setRecordValue(record, NotificationsModelFields::BuyRequest, i_id);
    if(client)
        m_notifications->setRecordValue(record, NotificationsModelFields::Client, client);
    if(repair)
        m_notifications->setRecordValue(record, NotificationsModelFields::RepairId, repair);
    m_notifications->setRecordValue(record, NotificationsModelFields::Subject, SNotification::typeToSubject(type));
    m_notifications->setRecordValue(record, NotificationsModelFields::Text, name());
    m_notifications->setRecordValue(record, NotificationsModelFields::State, 0);
}

void SPartRequest::translateNames()
{
    tr("NewUncommitted");
    tr("Created");
    tr("Finished");
    tr("Suspended");
    tr("InWork");
    tr("Cancelled");
    tr("InBasket");
    tr("OrderSubmitted");
    tr("OrderConfirmed");
    tr("Payed");
    tr("Shipped");

    tr("Regular");
    tr("High");
    tr("Low");
}
