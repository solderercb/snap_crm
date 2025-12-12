#include "srequest.h"
#include <QSqlTableModel>
#include <QMetaEnum>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SNotification>
#include <SComSettings>
#include <SPermissions>
#include <SSqlQueryModel>
#include <SUserSettings>
#include <SEditableBaseModel>
#include <SStandardItemModel>
#include <SLogRecordModel>

SPartRequest::SPartRequest(QObject *parent) :
    SSingleRowJModel{parent}
{
    mapFields();

    i_obligatoryFields << "request_time" << "from_user" << "state" << "item_name" << "notes" << "url" << "pririty";
    i_tableName = "parts_request";
    setPrimaryKeyIndex(0);

    m_record->setValue(C_state, State::NewUncommitted);

    i_logRecord->set_type(SLogRecordModel::RecordTypes::PartRequest);

//    int m_state = State::NewUncommitted;

    m_managers = new SEditableBaseModel(this, QSqlDatabase::database(TdConn::session()));
    m_managers->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_managers->setTable("parts_request_employees");

    m_notifications = new SEditableBaseModel(this, QSqlDatabase::database(TdConn::session()));
    m_notifications->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_notifications->setTable("notifications");

    connect(m_managers, &SEditableBaseModel::primeInsert, this, &SPartRequest::managersPrimeInsert);
    connect(m_notifications, &SEditableBaseModel::primeInsert, this, &SPartRequest::notificationsPrimeInsert);
    connect(this, &SSingleRowModel::beginDataChange, this, &SPartRequest::setDataRework);
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
    model->setQuery(QUERY_SEL_PRT_RQST_MANAGERS(SPermissions::C_enableSparepartRequestManager), QSqlDatabase::database(TdConn::main()));
    model->setObjectName("partRequestManagersModel");

    return model;
}

QString SPartRequest::constructSelectQuery()
{
    return QUERY_SEL_PART_REQUEST(id());
}

void SPartRequest::load()
{
    beginUpdateModel();

    SSingleRowModelBase::load();
    updateAdditionalModelsFilters(this->id());
    m_managers->select();
    m_notifications->select();

    endUpdateModel();
}

void SPartRequest::load(const int &id, QSqlDatabase &db)
{
    if(db.isValid())
        setDatabase(db);

    setPrimaryKey(id);

    load();
}

void SPartRequest::load(const int &id)
{
    QSqlDatabase db = QSqlDatabase();
    load(id, db);
}

void SPartRequest::stateChanged(const int newState)
{
    if(!isPrimaryKeyValid())
    {
        //  (NewUncommitted -» Created)
        if(newState == Created)
            initMandatoryField(C_created, QDateTime::currentDateTime());
    }
    else    //  (Any -» Created)
        appendLogText(logMsgStateChange(newState));

    if(newState == Finished)
        set_endDate(QDate::currentDate());
}

QString SPartRequest::logMsgStateChange(const int &newState)
{
    return tr("Статус изменён на %1").arg(partRequestStatesListModel->getDisplayRole(newState));
}

void SPartRequest::planEndDateChanged(const QDate &date)
{
    if(!isPrimaryKeyValid())
        return;

    // эти записи в журнал не производятся при создании заявки, а только при изменении
    auto old = commitedData(C_planEndDate).value_or(QVariant(QVariant::DateTime));
    QString logText;
    if(!old.toDateTime().isValid())
        logText = tr("Указана плановая дата завершения: %1").arg(date.toString("dd.MM.yyyy"));
    else
        logText = logMsgPlanEndDateChange(date);

    appendLogText(logText);
}

QString SPartRequest::logMsgPlanEndDateChange(const QDate &date)
{
    return tr("Плановая дата завершения изменёна на %1").arg(date.toString("dd.MM.yyyy"));
}

void SPartRequest::amountChanged(const double &amount)
{
    if(!isPrimaryKeyValid())
        return;

    auto old = commitedData(C_amount).value_or(QVariant(QVariant::Double));
    QString logText;
    if(old.isNull())
        logText = tr("Указана сумма: %1").arg(sysLocale.toCurrencyString(amount));
    else
        logText = logMsgAmountChange(amount);

    appendLogText(logText);
}

QString SPartRequest::logMsgAmountChange(const double &amount)
{
    return tr("Сумма изменёна на %1").arg(sysLocale.toCurrencyString(amount));
}
void SPartRequest::trackingChanged(const QString &tracking)
{
    auto old = commitedData(C_tracking).value_or(QVariant(QVariant::String));
    QString logText;
    logText = logMsgTrackingChange(tracking, old.toString());

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

void SPartRequest::priorityChanged(const int &priority)
{
    if(!isPrimaryKeyValid())
        return;

    appendLogText(logMsgPriorityChange(priority));
}

QString SPartRequest::logMsgPriorityChange(const int &priority)
{
    return tr("Приоритет изменён на %1").arg(partRequestPrioritiesListModel->getDisplayRole(priority));
}

void SPartRequest::countChanged(const int &count)
{
    if(!isPrimaryKeyValid())
        return;

    appendLogText(logMsgCountChange(count));
}

QString SPartRequest::logMsgCountChange(const int &count)
{
    return tr("Количество изменёно на %1").arg(count);
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

    filter = QString("`request` = %1").arg(id);
    if(filter.compare(m_managers->filter()))
        m_managers->setFilter(filter);

    filter = QString("`part_request_id` = %1").arg(id);
    if(filter.compare(m_notifications->filter()))
        m_notifications->setFilter(filter);
}

QList<int> SPartRequest::managers()
{
    QList<int> list;

    if(!isPrimaryKeyValid())
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
    Q_ASSERT_X(id(), "SPartRequest::setManagers()", "request id not set; porbably this method invoked before method commit()");
    updateAdditionalModel(m_managers, managers, ManagersModelFields::Employee);
    if(comSettings->notifyItemPurchaseRequest())
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
    list.removeOne(userDbData->id()); // уведомление самому себе не нужно
    list.removeOne(employee()); // уведомление инициатору не нужно
    updateAdditionalModel(m_notifications, list, NotificationsModelFields::Employee);
}

void SPartRequest::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
        case C_state: stateChanged(data.toInt()); break;
        case C_planEndDate: planEndDateChanged(data.toDate()); break;
        case C_amount: amountChanged(data.toDouble()); break;
        case C_tracking: trackingChanged(data.toString()); break;
        case C_priority: priorityChanged(data.toInt()); break;
        case C_count: countChanged(data.toInt()); break;
        default: break;
    }
}

bool SPartRequest::commit()
{
    if(!isPrimaryKeyValid())
    {
        QStringList logs;
        logs.append(tr("Заявка на закупку %1ед. \"%2\" создана").arg(count()).arg(name()));
        logs.append(tr("Приоритет: %1").arg(partRequestPrioritiesListModel->getDisplayRole(priority())));
        if(planEndDate().isValid())
            logs.append(tr("Deadline: %1").arg(planEndDate().toString(sysLocale.dateFormat(QLocale::ShortFormat))));
        appendLogText(logs.join(". "), ".");

        initMandatoryField(C_notes, "");
        initMandatoryField(C_url, "");
    }

    return SSingleRowJModel::commit();
}

bool SPartRequest::isManagersModelDirty()
{
    return m_managers->isDirty();
}

bool SPartRequest::commitManagers()
{
    SSingleRowModel::checkSystemTime();

    if(!m_managers->submitAll())
        Global::throwError(m_managers->lastError(), tr("Не удалось сохранить ответственных сотрудников в заявке на закупку"));
    if(!m_notifications->submitAll())
        Global::throwError(m_notifications->lastError(), tr("Не удалось отправить уведомления ответственным сотрудникам при сохранении заявки на закупку"));

    return 1;
}

/* Переопределённый метод
 * Установка ID запроса на закупку в поле `logs`.`part_request`
*/
void SPartRequest::updateLogAssociatedRecId()
{
    i_logRecord->set_partRequest(id());
}

void SPartRequest::managersPrimeInsert(int row, QSqlRecord &record)
{
    Q_UNUSED(row)

    m_managers->setRecordValue(record, ManagersModelFields::RequestId, id());
}

void SPartRequest::notificationsPrimeInsert(int row, QSqlRecord &record)
{
    Q_UNUSED(row)

    int type = SNotification::Type::BuyRequest;
    int client = this->client();
    int repair = this->repair();

    m_notifications->setRecordValue(record, NotificationsModelFields::Created, QDateTime::currentDateTimeUtc());
    m_notifications->setRecordValue(record, NotificationsModelFields::Employee, employee());
    m_notifications->setRecordValue(record, NotificationsModelFields::Type, type);
    if(id())
        m_notifications->setRecordValue(record, NotificationsModelFields::BuyRequest, id());
    if(client)
        m_notifications->setRecordValue(record, NotificationsModelFields::Client, client);
    if(repair)
        m_notifications->setRecordValue(record, NotificationsModelFields::RepairId, repair);
    m_notifications->setRecordValue(record, NotificationsModelFields::Subject, SNotification::typeToSubject(type));
    m_notifications->setRecordValue(record, NotificationsModelFields::Text, name());
    m_notifications->setRecordValue(record, NotificationsModelFields::State, 0);
}

void SPartRequest::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_repair:
        case C_dealer:
        case C_client: if(!data.toInt()) data = QVariant(QVariant::Int); break;
        case C_amount: if(data.toDouble() == 0) data = QVariant(QVariant::Double); break;
        case C_notes:
        case C_tracking: if(data.toString().isEmpty()) data = QVariant(QVariant::String); break;
        default: ;
    }
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

