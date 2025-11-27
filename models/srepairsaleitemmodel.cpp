#include "srepairsaleitemmodel.h"
#include <QSqlQuery>
#include <SStandardItemModel>
#include <SSqlQueryModel>
#include <SSaleTableModel>
#include <SUserSettings>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SStoreItemModel>
#include <SLogRecordModel>

SRepairSaleItemModel::SRepairSaleItemModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "store_int_reserve";
    i_obligatoryFields << "item_id" << "count" << "from_user" << "to_user" << "notes" << "state" << "price";

    setPrimaryKeyIndex(0);
    i_logRecord->set_type(SLogRecordModel::Repair);
    connect(this, &SSingleRowModel::beginDataChange, this, &SRepairSaleItemModel::setDataRework);

    m_storeItem = std::make_shared<SStoreItemModel>();
    m_storeItem->setSaleMode(SStoreItemModel::SaleMode::Repair);
}

SRepairSaleItemModel::~SRepairSaleItemModel()
{
}

int SRepairSaleItemModel::count_()
{
    if( id() == 0 )
        return 0;

    bool nErr = 1;
    QUERY_EXEC(i_query, nErr)( QUERY_SEL_REPAIR_SALES_QTY(id()));
    i_query->first();
    return i_query->value(0).toInt();
}

bool SRepairSaleItemModel::request()
{
    set_state(State::Requested);
    // TODO: создание уведомления ответственному сотруднику
    commit();

    return 1;
}

/*  Выдача товара сотруднику
 *  Может быть инициирована ответственным сотрудником или как ответ на запрос детали
*/
bool SRepairSaleItemModel::reserve()
{
    auto commitedState = commitedData(C_state).value_or(State::New).toInt();
    switch (commitedState)
    {
        case State::New:
        case State::Requested: m_storeItem->reserve(notes()); break;
        default: break;
    }
    if(cacheRef(C_count)->state() == ModifiedField::Updated)
        m_storeItem->reserve(notes());

    return 1;
}

bool SRepairSaleItemModel::sale()
{
    set_state(State::Sold);
    commit();
    m_storeItem->saleRepair(tr("Продажа %1ед. товара в связи с выдачей ремонта №%2.").arg(count()).arg(repairId()));
    return 1;
}

bool SRepairSaleItemModel::unsale()
{
    set_state(State::RepairLinked);
    commit();
    m_storeItem->unsaleRepair(tr("Возврат %1ед. товара по причене отмены выдачи ремонта №%2.").arg(count()).arg(repairId()));
    return 1;
}

bool SRepairSaleItemModel::unlinkRepair()
{
    initMandatoryField(C_state, State::EngineerBasket); // статус задаётся при инициализации модели в конструкторе; если удаление происходит на вкладке tabRepairSaleItem то нужно задать статус
    commitLogs();
    set_repairId(0);
    set_workId(0);
    commit();
    return 1;
}

bool SRepairSaleItemModel::free()
{
    set_state(State::Archive);
    commit();
    m_storeItem->free(tr("Возврат %1ед. товара на склад").arg(count()));
    return 1;
}

bool SRepairSaleItemModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_created, QDateTime::currentDateTime());
        initMandatoryField(C_fromUser, userDbData->id());
        initMandatoryField(C_toUser, userDbData->id());
//        initMandatoryField(C_notes, tr("Выдача товара \"%1\" сотруднику %2").arg(name(), QSqlDatabase::database("connMain").userName()));
    }

    switch(state())
    {
        case State::EngineerBasket:
        case State::RepairLinked: reserve(); break;
    }

    m_storeItem->commitLogs(); // сообщения в журнал, если ранее они не были записаны

    return SSingleRowJModel::commit();
}

void SRepairSaleItemModel::updateLogAssociatedRecId()
{
    // таблица store_int_reserve не имеет связей с таблицей logs
    // метод переопределён для подавления предупреждения
}

/* Загрузка данных из таблицы `store_int_reserve` и инициализация полей модели SStoreItemModel
 * Метод будет использоваться в классе вкладки управления товаром в корзине сотрудника:
 *  - подтверждение запроса запчасти и резервирование
 *  - возврат на склад
 *  - списание
 *  и др.
 *  TODO: разработка не завершена!
*/
void SRepairSaleItemModel::load()
{
    SSingleRowModel::load();
    m_storeItem->setPrimaryKey(this->itemId());
    m_storeItem->setCommitedExpense(count());
    m_storeItem->setNewExpense(count());
}

void SRepairSaleItemModel::countChanged(const QVariant &value)
{
    m_storeItem->setNewExpense(value.toInt());

    int state = this->state();
    if(state == State::New || state == State::Requested)
    {
        constructNote();
        return;
    }

    if(!isPrimaryKeyValid())
    {
        addRemoveHook();
        return;
    }

    auto old = commitedData(C_count);

    // запись в журнал ремонта (кол-во можно изменить только в режиме админ. правки)
    if(state == State::RepairLinked)
    {
        appendLogText(tr("[А] Количество товара \"%1\" изменёно с %2 на %3").arg(name()).arg(old->toInt()).arg(value.toInt()));
    }

    if(state == State::EngineerBasket || state == State::RepairLinked)
    {
        m_storeItem->appendLogText(tr("Количество ранее зарезервированного товара изменено с %1 на %2 (id заявки: %3)")
                                   .arg(old->toInt())
                                   .arg(value.toInt())
                                   .arg(id()),
                                   "0");
    }
}

void SRepairSaleItemModel::engineerChanged()
{
    int state = this->state();
    if(state == State::New || state == State::Requested)
    {
        constructNote();
        return;
    }

    auto old = commitedData(C_toUser).value_or(0).toInt();
    // запись о изменении относится к товару
    m_storeItem->appendLogText(tr("[A] Товар \"%1\" перемещён из корзины сотрудника %2 в корзину сотрудника %3")
                               .arg(name(),
                               usersModel->getDisplayRole(old),
                               usersModel->getDisplayRole(toUser())),
                               "engChanged");
}

void SRepairSaleItemModel::repairChanged(const QVariant &value)
{
    i_logRecord->setData(SLogRecordModel::C_repair, value);
    addRemoveHook(); // если при изменении статуса номер ремонта еще не был задан (например, быстрый ремонт)
}

void SRepairSaleItemModel::priceChanged(const QVariant &value)
{
    int state = this->state();
    if(state == State::New || state == State::Requested)
        return;

    if(!isPrimaryKeyValid())
    {
        addRemoveHook();
        return;
    }

    SSaleTableModel *model = static_cast<SSaleTableModel*>(parent());

    auto old = commitedData(C_price);
    QString logText = tr("Стоимость товара \"%1\" изменена с %2 на %3").arg(name(), sysLocale.toCurrencyString(old->toDouble()), sysLocale.toCurrencyString(value.toDouble()));

    if(model)
    {
        if(model->state() == WorkshopSaleModel::State::Adm && !logText.isEmpty())
            logText.prepend("[A] ");
    }

    appendLogText(logText);
}

/* Генерация записей в журнал при смене статуса и/или связанного номера ремонта и
 * инициализация полей объекта SStoreItemModel
*/
void SRepairSaleItemModel::addRemoveHook()
{
    if(!repairId()) // номер ремонта является обязательным для объекта SLogRecordModel
        return;

    if(!cache.contains(C_state) || cacheRef(C_state)->state() != ModifiedField::Updated)
    {
        // если пользователь в режиме ManualSubmit ошибся и пометил на удаление не ту строку, то при повторном нажатии на
        // иконку для отмены нужно удалить сгенерированную для журнала запись (см. алгоритм метода SSingleRowModelBase::setData)
        i_logTexts.remove("!stateAddRem");
        return;
    }

    int state = this->state();
    QString text("");
    auto oldState = commitedData(C_state).value_or(-1).toInt();

    if(state == State::EngineerBasket || state == State::Archive)
    {
        if(oldState == State::RepairLinked)
        {
            text = tr("Удален товар \"%1\" стоимостью %2 в кол-ве %3ед.")
                        .arg(name(),
                        sysLocale.toCurrencyString(price()))
                        .arg(count());
        }
    }
    else if(state == State::RepairLinked)
    {
        if(oldState == State::Sold) // в случае отмены выдачи ремонта запись в жрурнал не должна производиться
            return;

        text = tr("Добавлен товар \"%1\" стоимостью %2 в кол-ве %3ед")
                    .arg(name(),
                    sysLocale.toCurrencyString(price()))
                    .arg(count());

//        if(!isPrimaryKeyValid())
//        {
//            m_storeItem->setNewExpense(count());
            constructNote();
//        }
    }

    if(text.length() == 0)
        return;

    if(parent())
    {
        SSaleTableModel *model = static_cast<SSaleTableModel*>(parent());
        if(model->state() == WorkshopSaleModel::State::Adm)
            text.prepend("[A] ");
    }

    appendLogText(text, "!stateAddRem");    // запись всегда самая первая
}

void SRepairSaleItemModel::constructNote()
{
    int state = this->state();
    QString t;
    if(state == State::New || state == State::Requested)
    {
        t = tr("Выдача %1ед. товара сотруднику %2")
            .arg(count())
            .arg(allUsersModel->getDisplayRole(toUser()));
    }
    else if(state == State::RepairLinked && !isPrimaryKeyValid())
    {
        t = tr("Автовыдача %1ед. товара сотруднику %2 для ремонта №%3")
                    .arg(count())
                    .arg(allUsersModel->getDisplayRole(userDbData->id()))
                    .arg(repairId());
    }
    else
        return;

    set_notes(t);
}

void SRepairSaleItemModel::serialChanged(const QVariant &value)
{
    if(!repairId())
        return;

    SSaleTableModel *model = static_cast<SSaleTableModel*>(parent());
    QString logText;

    auto old = commitedData(C_sn).value_or("").toString();
    if(!old.isEmpty())
        logText = tr("Серийный номер товара \"%1\" изменён с \"%2\" на \"%3\"").arg(name(), old, value.toString());
    else if (!value.toString().isEmpty())
        logText = tr("Серийный номер товара \"%1\" установлен \"%2\"").arg(name(), value.toString());

    if(model)
    {
        if(model->state() == WorkshopSaleModel::State::Adm && !logText.isEmpty())
            logText.prepend("[A] ");
    }

    appendLogText(logText);
}

void SRepairSaleItemModel::warrantyChanged(const QVariant &value)
{
    if(!repairId())
        return;

    SSaleTableModel *model = static_cast<SSaleTableModel*>(parent());
    QString logText;

    auto old = commitedData(C_warranty);
    if(old)
        logText = tr("Срок гарантии на товар \"%1\" изменён с \"%2\" на \"%3\"").arg(name(), warrantyTermsModel->getDisplayRole(old->toInt(), 1), warrantyTermsModel->getDisplayRole(value.toInt(), 1));
    else
        logText = tr("Срок гарантии на товар \"%1\" установлен \"%2\"").arg(name(), warrantyTermsModel->getDisplayRole(value.toInt(), 1));

    if(model)
    {
        if(model->state() == WorkshopSaleModel::State::Adm && !logText.isEmpty())
            logText.prepend("[A] ");
    }

    appendLogText(logText, "F"); // disambiguation здесь нужен, чтобы запись была последней
}

void SRepairSaleItemModel::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_toUser: if(state() == Sold) data = this->toUser(); break; // изменять пользователя в выданом ремонте нельзя
        case C_workId: if(!data.toInt()) data = QVariant(); break;
        case C_repairId: if(!data.toInt()) data = QVariant(); break;
        default: ;
    }
}

void SRepairSaleItemModel::logDataChange(const int index, const QVariant &data)
{
    switch(index)
    {
        case C_count: countChanged(data); break;
        case C_toUser: engineerChanged(); break;
        case C_state: addRemoveHook(); break;
        case C_repairId: repairChanged(data); break;
        case C_price: priceChanged(data); break;
        case C_sn: serialChanged(data); break;
        case C_warranty: warrantyChanged(data); break;
        default: ;
    }
}

/* Переопределённый метод.
 * Дополнительно к базовому функционалу инициализирует поле SStoreItemModel
*/
bool SRepairSaleItemModel::setData(const int index, const QVariant &value)
{
    if(index < 0)
        return 1;

    if(index == Columns::C_itemId)
        m_storeItem->setPrimaryKey(value);

    return SSingleRowJModel::setData(index, value);
}

void SRepairSaleItemModel::initFieldWithPrevLoaded(const int index, const QVariant &value)
{
    SSingleRowJModel::initFieldWithPrevLoaded(index, value);

    switch (index)
    {
        case C_itemId: m_storeItem->setPrimaryKey(value); break;
        case C_count: m_storeItem->setCommitedExpense(value.toInt()); break;
        case C_repairId: repairChanged(value); break;
        default: break;
    }
}
