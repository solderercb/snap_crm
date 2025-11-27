#include "sclientmodel.h"
#include <QMetaEnum>
#include <QRandomGenerator>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SPermissions>
#include <SComSettings>
#include <SUserSettings>
#include <SPhoneModel>
#include <SPhonesModel>
#include <SSqlQueryModel>
#include <SLogRecordModel>

SClientModel::SClientModel(int id, QObject *parent) :
    SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "name" << "notes";
    i_tableName = "clients";
    setPrimaryKeyIndex(0);
    connect(this, &SSingleRowModel::beginDataChange, this, &SClientModel::setDataRework);

    m_phones = new SPhonesModel();

    i_logRecord->set_type(SLogRecordModel::Client);

    if(id)
    {
        load(id);
    }
}

SClientModel::~SClientModel()
{
    delete m_phones;
    deleteBalanceObj();
}

SStandardItemModel *SClientModel::categoriesList()
{
    SStandardItemModel *list;
    QMetaEnum types = SClientModel::staticMetaObject.enumerator(SClientModel::staticMetaObject.indexOfEnumerator("Categories"));
    QVector<QString> clientTypesQueryFilterList = {"`type` IN (0, 1)",
                                                   "`type` = 1",
                                                   "`is_agent` = 1",
                                                   "`is_dealer` = 1",
                                                   "`is_regular` = 1",
                                                   "`is_bad` = 1",
                                                   "`is_realizator` = 1"};

    Q_ASSERT_X(types.keyCount() == clientTypesQueryFilterList.count(), "SClientModel::categoriesList()", "count not match");

    list = SStandardItemModel::modelFromEnum(types, tr);
    list->setColumnCount(3);
    for(int i = 0; i < list->rowCount(); i++)
    {
        list->setData(list->index(i, 2), clientTypesQueryFilterList.at(i));
    }
    list->setObjectName("clientsCategoriesList");
//    list->setObjectName("clientsTypesList");

    return list;
}

bool SClientModel::isNew()
{
    return !isPrimaryKeyValid();
}

QString SClientModel::fieldsForSelectQuery()
{
    QString fields;

    // TODO: заменить "no permissions" на частично скрытое значение (например, "Имя Ф.", как при переводе денег на карту)
    auto viewClient = [=](int index){if(!permissions->viewClients) return restrictedFieldForSelectQuery(index); return fieldForSelectQuery(index);};
    auto viewPassport = [=](int index){if(!permissions->viewClientPassportData) return restrictedFieldForSelectQuery(index); return fieldForSelectQuery(index);};
    for(int i = 0; i < rec()->count(); i++)
    {
        if(!fields.isEmpty())
            fields.append(",\n");

        switch (i)
        {
            case C_firstName:
            case C_lastName:
            case C_patronymicName:
            case C_address:
            case C_email: fields.append(viewClient(i)); break;
            case C_passportNum:
            case C_passportIssuedDate:
            case C_passportIssuedBy:
            case C_INN:
            case C_KPP:
            case C_OGRN: fields.append(viewPassport(i)); break;
            default: fields.append(fieldForSelectQuery(i)); break;
        }
    }

    return fields;
}

void SClientModel::load()
{
    SSingleRowJModel::load();
    m_phones->load(id());
    i_logRecord->set_client(id());
    if(permissions->viewClients && !notes().isEmpty() && !m_notificationShown)
    {
        m_notificationShown = 1;
        showNotification(notes(), QMessageBox::Information);
    }
    if(isBalanceEnabled())
    {
        deleteBalanceObj(); // при перезагрузке данных клиента объект должен быть удалён
        createBalanceObj();
    }
}

void SClientModel::load(int id)
{
    setPrimaryKey(id);
    load();
}

void SClientModel::initDemo()
{
    m_record->setValue(C_id, 123);
    m_record->setValue(C_firstName, "Андрей");
    m_record->setValue(C_lastName, "Андреев");
    m_record->setValue(C_patronymicName, "Андреевич");
    m_record->setValue(C_address, "пр. Дружбы народов, д. 5");
    m_record->setValue(C_postIndex, "123000");
    m_record->setValue(C_passportNum, "111111");
    m_record->setValue(C_passportIssuedDate, QDate::currentDate().toString("dd.MM.yyyy"));
    m_record->setValue(C_passportIssuedBy, "АБ ВГД Центрального района");
    m_record->setValue(C_state, 1);
    m_record->setValue(C_isCompany, 0);
    m_record->setValue(C_birthday, QDate::currentDate().toString("dd.MM.yyyy"));
    m_record->setValue(C_notes, "делать скидки на ремонт");
    m_record->setValue(C_isRegular, 1);
    m_record->setValue(C_adType, clientAdTypesList->index(0, 1).data().toInt());
    m_record->setValue(C_INN, "1234567890");
    m_record->setValue(C_KPP, "4567");
    m_record->setValue(C_OGRN, "123789");
    m_record->setValue(C_web_password, "FC9Y76U3");
    m_record->setValue(C_urName, "");
    m_record->setValue(C_email, "admin@example.com");
    m_record->setValue(C_skype, "user_skype");
    m_record->setValue(C_site, "example.com");
    m_record->setValue(C_balance, 999999);
    m_record->setValue(C_priceColumn, priceColModel->index(0, 1).data().toInt());
    m_record->setValue(C_repairs, 10);
    m_record->setValue(C_purchases, 20);
    m_record->setValue(C_created, QDateTime::currentDateTime());
}

/* Состояние флага "Акт выполненных работ" по умолчанию.
 * Метод используется на вкладке Приём в ремонт (быстрый ремонт) и в диалоге выдачи устройства.
 * Состояние флага зависит от настройки печати по умолчанию (Настройки -» Основные -» Печать документов по умолчанию) и
 * от параметров карточки клиента: для постоянных клиентов, клиентов с включенным балансом и организаций он по умолчанию выключен.
*/
bool SClientModel::printBOQDefaultState()
{
    // TODO: возможно, стоит добавить дополнительную настройку для включения этого т. н. расширенного режима
    // TODO: возможно, стоит добавить дополнительное свойство в карточку клиента, указывающее на обязательную печать Акта вып. работ
    //       (например, большинству постоянных клиентов акт не нужен, но есть один особый клиент, которому он нужен обязательно)
    return comSettings->printWorksList() && !(isBalanceEnabled() | isCompany() | isRegular());
}

void SClientModel::clear()
{
    clearEverything();
    deleteBalanceObj();
    m_phones->reset();

    emit modelUpdated();
}

void SClientModel::setEditStrategy(EditStrategy strategy)
{
    m_editStrategy = strategy;
}

bool SClientModel::isStandAlone()
{
    return m_standAlone;
}

QVariant SClientModel::fullLongName()
{
    if(!permissions->viewClients)
        return tr("no permissions"); // TODO: заменить "no permissions" на "Имя Ф." (как при переводе денег на карту)

    QString ret;
    if(isCompany())
    {
        if(!urName().isEmpty())
            ret = urName();
        else
            ret = firstName();
    }
    else
    {
        ret = lastName();
        if(!firstName().isEmpty())
            ret.append(" " + firstName());
        if(!patronymicName().isEmpty())
            ret.append(" " + patronymicName());
    }

    return ret;
}

QVariant SClientModel::fullShortName()
{
    if(!permissions->viewClients)
        return tr("no permissions");

    QString ret;
    if(isCompany())
    {
        return fullLongName();
    }
    else
    {
        ret = lastName();
        if(!firstName().isEmpty())
            ret.append(" ").append(firstName().front()).append(".");
        if(!patronymicName().isEmpty())
            ret.append(" ").append(patronymicName().front()).append(".");
    }

    return ret;
}

QVariant SClientModel::isArchived()
{
    return !state();
}

SPhonesModel* SClientModel::phones()
{
    return m_phones;
}

void SClientModel::addPhone(const QString &number, int comboBoxMaskIndex, int messengers)
{
    SPhoneModel *phone = new SPhoneModel();
    phone->set_phone(number);
    phone->set_maskIndex(comboBoxMaskIndex);
    if(m_phones->isEmpty())
        phone->set_primary();
    phone->set_messengers(messengers);
    m_phones->add(phone);
}

/*  Список свойств клиента, которые необходимо отображать в карточке ремонта
 *  или в графе с данными клиента при приёме в ремонт (в QListWidget)
*/
QStringList SClientModel::optionsList(int format)
{
    int col = 0;
    bool flag;
    if(format == OptionsOutputForm::Short)
        col = 3;
    QStringList list;
    for(int i = 0; i < clientBinaryProperties->rowCount(); i++)
    {
        switch(1<<i)
        {
//            case BinaryOption::Company:   // type()
//            case BinaryOption::BalanceEnabled:    // balanceEnabled()
//            case BinaryOption::Archived:  //  state()
            case BinaryOption::Supplier: flag = isSupplier(); break;
            case BinaryOption::SaleOrReturn: flag = isGivesItemsForSale(); break;
            case BinaryOption::Regular: flag = isRegular(); break;
            case BinaryOption::Broker: flag = isBroker(); break;
            case BinaryOption::IgnoreCalls: flag = isIgnoreCalls(); break;
            case BinaryOption::PreferCashless: flag = isPreferCashless(); break;
            case BinaryOption::TakeLong: flag = isTakeLong(); break;
            case BinaryOption::Bad: flag = isBad(); break;
            default: flag = 0;
        }
        if(flag)
            list << clientBinaryProperties->index(i, col).data().toString();
    }
    return list;
}


void SClientModel::setAdTypeIndex(const int index)
{
    if(index == -1)
        setData(C_adType, QVariant());
    else
        set_adType(clientAdTypesList->databaseIDByRow(index));
}

int SClientModel::adTypeIndex()
{
    return clientAdTypesList->rowByDatabaseID(adType(), 1);
}

QVariant SClientModel::adTypeStr()
{
    return clientAdTypesList->getDisplayRole(adType(), "id");
}

QVariant SClientModel::photo()
{
    return QByteArray();
}

void SClientModel::setDataRework(const int index, QVariant &data)
{
    switch (index)
    {
        case C_isBalanceEnabled: if(isPrimaryKeyValid()) data = isBalanceEnabled(); break; // подавление прямого изменения; для включения/выключения необходимо вызвать метод setBalanceEnabled()
        default: break;
    }

}

/*  Включение/отключение баланса клиента
 *  Возвращает 0 в случае ошибки
*/
bool SClientModel::setBalanceEnabled(bool state)
{
    if( state != isBalanceEnabled() )
    {

        if(state)
        {
            appendLogText(tr("Включен баланс клиента"));
            createBalanceObj();
        }
        else
        {
            if(balance() != 0)
            {
                // TODO: переделать с использованием shortlivedNotification
                // или предложить пользователю автоматически создать РКО/ПКО
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Отключение баланса"));
                msgBox.setText(tr("Баланс клиента не равен нулю!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
                return 0;
            }
            else
            {
                appendLogText(tr("Баланс клиента отключен"));
                deleteBalanceObj();
            }
        }
        blockSignals(true);
        set_isBalanceEnabled(state);
        blockSignals(false);

        commit();
    }
    return 1;
}

void SClientModel::loadBalance()
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird")); // это не ошибка, используется соединение для записи в БД
    query->exec(wrapSelectQueryFields(fieldForSelectQuery(C_balance)));
    query->first();
    if(query->isValid())
        m_record->setValue(C_balance, query->value(0));
}

QString SClientModel::balanceStr()
{
    return sysLocale.toCurrencyString(balance());   // TODO: код валюты для мультивалютных балансов/касс
}

void SClientModel::createBalanceObj()
{
    balanceLog = new SBalanceLogRecordModel(id());
    loadBalance();
}

SBalanceLogRecordModel *SClientModel::balanceObj()
{
    return balanceLog;
}

void SClientModel::deleteBalanceObj()
{
    if(balanceLog)
        delete balanceLog;
    balanceLog = nullptr;
}

bool SClientModel::updateBalance(const double amount, const QString &text)
{
    if(amount == 0)
        return 1;

    if(isNew())   // TODO: проверка включен ли баланс у клиента
        Global::throwError(Global::ThrowType::InputError);

    bool nErr = 1;
    balanceLog->set_reason(text);
    balanceLog->set_client(id());
    // Выбор офиса пользователем не предусмотрен; используется текущее значение из класса userDbData
    nErr &= balanceLog->commit(amount);
    double newAmount = balance() + amount;

    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));
    checkSystemTime();
    set_balance(newAmount);
    QString q = wrapUpdateQueryPairs(fieldForUpdateQuery(C_balance) + fieldValueHandler(newAmount));

    QUERY_EXEC_TH(query, nErr, q);
    setState(C_balance, SSingleRowModelBase::ModifiedField::Executed);

    QUERY_EXEC_TH(query,nErr,QUERY_VRFY_BALANCE(id()));
    QUERY_EXEC_VRFY(query,id());

    return nErr;
}

bool SClientModel::updateBalance(const double amount, const QString &text, const SBalanceLogRecordModel::RoyaltyReason reason, const int reason_id)
{
    switch(reason)
    {
        case SBalanceLogRecordModel::RoyaltyReason::Repair: balanceLog->setRepair(reason_id); break;
        case SBalanceLogRecordModel::RoyaltyReason::Document: balanceLog->setDocumentId(reason_id); break;
        case SBalanceLogRecordModel::RoyaltyReason::CashOrder: balanceLog->setCashOrderId(reason_id); break;
    }

    return updateBalance(amount, text);
}

/* Проверка достаточности средств на балансе
 * Используется при выдаче ремонта или продаже товара с оплатой с баланса
 * Возвращает 1 если средсв достаточно или пользователь подтвердил "уход в минус"
*/
bool SClientModel::balanceEnough(const double amount)
{
    if( amount < 0 && balance() >= -amount )
        return 1;

    QMessageBox resBtn( QMessageBox::Question, "SNAP CRM",
                        tr("На балансе недостаточно средств, продолжить?"),
                        QMessageBox::No | QMessageBox::Yes);
    QAbstractButton *noButton = resBtn.button(QMessageBox::No);
    resBtn.setDefaultButton(QMessageBox::No);
    resBtn.setWindowModality(Qt::ApplicationModal);
    resBtn.exec();
    if (resBtn.clickedButton() == noButton)
        return 0;

    return 1;
}

int SClientModel::priceColumnIndex()
{
    if(!priceColumn())  // на случай ошибок в БД, по умолчанию тип цен — Розница
        return priceColModel->rowByDatabaseID(2, 1);

    return priceColModel->rowByDatabaseID(priceColumn(), 1);
}

QString SClientModel::priceColumnStr()
{
    return priceColModel->getDisplayRole(priceColumn(), "id");
}

void SClientModel::updateRepairs(const int val)
{
    if(!isPrimaryKeyValid())
        return;

    bool nErr = 1;
    auto sql = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));
    QString query = wrapSelectQueryFields(fieldForSelectQuery(C_repairs));
    QUERY_EXEC_TH(sql, nErr, query);
    sql->first();
    m_record->setValue(C_repairs, sql->value(0).toInt());
    set_repairs(repairs() + val);
    commit();
}

void SClientModel::updatePurchases(int items)
{
    if(!isPrimaryKeyValid())
        return;

    // в АСЦ с кол-вом покупок то ли глюк, то ли странная задумка: в таблице клиентов отображается кол-во РН, а в самой карте клиента кол-во записей в таблице store_sales
    // TODO: разобраться с этим
    bool nErr = 1;
    auto sql = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));
    QString query = wrapSelectQueryFields(fieldForSelectQuery(C_purchases));
    QUERY_EXEC_TH(sql, nErr, query);
    sql->first();
    m_record->setValue(C_purchases, sql->value(0).toInt());
    set_purchases(purchases() + items);
    commit();
}

/* Поступление денег на баланс клиента
 */
bool SClientModel::receiptBalance()
{
    return 0;
}

/* Выдача денег с баланса клиента
 */
bool SClientModel::paymentBalance()
{
    return 0;
}

void SClientModel::showNotification(const QString &message, const QMessageBox::Icon icon)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle(fullShortName().toString());
    msgBox.setText(message);
    msgBox.setIcon(icon);
    msgBox.exec();
}

// https://stackoverflow.com/a/18866593
QString SClientModel::genWebPass()
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    const int randomStringLength = 6; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

bool SClientModel::commit()
{
    bool nErr = 1;

    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_created, QDateTime::currentDateTime());
        initMandatoryField(C_web_password, genWebPass());
        initMandatoryField(C_creator, userDbData->id());
        initMandatoryField(C_notes, "");
    }

    nErr &= SSingleRowJModel::commit();

    m_phones->setClient(id());
    nErr &= m_phones->commit();

    return nErr;
}

void SClientModel::updateLogAssociatedRecId()
{
    i_logRecord->set_client(id());
}

SBalanceLogRecordModel::SBalanceLogRecordModel(QObject *parent):
    SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "client" << "summ" << "direction" << "reason" << "created" << "office" << "uid";
    i_tableName = "balance";
    setPrimaryKeyIndex(0);
    setQueryIdBeforeInsert(false);
    i_logRecord->set_type(SLogRecordModel::Client);
}

SBalanceLogRecordModel::SBalanceLogRecordModel(int client, QObject *parent):
    SBalanceLogRecordModel(parent)
{
    set_client(client);
}

SBalanceLogRecordModel::~SBalanceLogRecordModel()
{

}

void SBalanceLogRecordModel::setRepair(const int id)
{
    i_logRecord->set_repair(id);
    i_logRecord->setData(SLogRecordModel::C_document, QVariant());
    i_logRecord->setData(SLogRecordModel::C_cashOrder, QVariant());
}

void SBalanceLogRecordModel::setDocumentId(const int id)
{
    i_logRecord->set_document(id);
    i_logRecord->setData(SLogRecordModel::C_repair, QVariant());
    i_logRecord->setData(SLogRecordModel::C_cashOrder, QVariant());
}

void SBalanceLogRecordModel::setCashOrderId(const int id)
{
    i_logRecord->set_cashOrder(id);
    i_logRecord->setData(SLogRecordModel::C_document, QVariant());
    i_logRecord->setData(SLogRecordModel::C_repair, QVariant());
}

void SBalanceLogRecordModel::setOfficeIndex(const int index)
{
    set_office(officesModel->databaseIDByRow(index, "id"));
}

/* Переопределённый метод
 * Требуется пометка только поля id. Остальные поля остаются в статусе Updated, т. к. может выполняться несколько записей подряд (например, при выдаче нескольких ремонтов).
 * В тестах это позволит проверить последнюю запись на предмет правильно отработавших алгоритмов коммита
*/
void SBalanceLogRecordModel::setAllState(ModifiedField::State state)
{
    if(!m_queryIdBeforeInsert)
        return;

    CacheMap::iterator i = cache.find(C_id);
    if(i != cache.end())
    {
        setState(*i, state);
    }
}

bool SBalanceLogRecordModel::commit()
{
    return SSingleRowJModel::commit();
}

bool SBalanceLogRecordModel::commit(const double amount)
{
    bool nErr = 1;

    set_amount(amount);
    initMandatoryField(C_user, userDbData->id());
    initMandatoryField(C_office, userDbData->currentOffice());
    initMandatoryField(C_created, QDateTime::currentDateTime());

#ifdef QT_DEBUG
    checkTableName();
#endif
    checkSystemTime();

    nErr &= insert();
    nErr &= i_logRecord->commit();
    setFieldsExecuted();

    return nErr;
}

bool SBalanceLogRecordModel::commit(const double amount, const QString &text)
{
    set_reason(text);

    return commit(amount);
}

void SBalanceLogRecordModel::updateLogAssociatedRecId()
{
    // запись в журнал об изменении баланса связана только с id клиента; его установка происходит при создании объекта
    // метод переопределён для подавления предупреждения
}

void SBalanceLogRecordModel::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
        case C_amount: set_direction(data.toDouble() > 0); break;
        case C_client: i_logRecord->set_client(data.toInt()); break;
        case C_reason: i_logRecord->setData(SLogRecordModel::C_text, data); break;
        default: break;
    }
}

void SClientModel::translateNames()
{
    tr("All");
    tr("Companies");
    tr("Brokers");
    tr("Suppliers");
    tr("Regulars");
    tr("Problematic");
    tr("Resellers");
}
