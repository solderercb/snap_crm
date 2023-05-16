#include "sclientmodel.h"

SClientModel::SClientModel(int id, QObject *parent) :
    SComRecord(parent)
{
    i_obligatoryFields << "name" << "notes";
    i_tableName = "clients";
    m_phones = new SPhonesModel();

    i_logRecord->setType(SLogRecordModel::Client);

    if(id)
    {
        load(id);
    }
    else
    {
        i_valuesMap.clear();
        i_valuesMap.insert("creator", userDbData->value("id"));
        i_valuesMap.insert("notes", "");
    }
}

SClientModel::~SClientModel()
{
    delete m_phones;
    deleteBalanceObj();
}

bool SClientModel::isNew()
{
    if(i_id)
        return false;

    return true;
}

void SClientModel::load(int id)
{
    QSqlQueryModel *clientModel = new QSqlQueryModel(this);
    clientModel->setQuery(QUERY_SEL_CLIENT(id), QSqlDatabase::database("connMain"));

    if(clientModel->rowCount())
    {
        i_id = id;
        i_valuesMap.clear();
        m_phones->load(i_id);
        i_logRecord->setClient(i_id);
        m_firstName = clientModel->record(0).value("name").toString();
        m_lastName = clientModel->record(0).value("surname").toString();
        m_patronymicName = clientModel->record(0).value("patronymic").toString();
        m_address = clientModel->record(0).value("address").toString();
        m_postIndex = clientModel->record(0).value("post_index").toString();
        m_passportNum = clientModel->record(0).value("passport_num").toString();
        m_passportIssuedDate = clientModel->record(0).value("passport_date").toString();
        m_passportIssuedBy = clientModel->record(0).value("passport_organ").toString();
        m_state = clientModel->record(0).value("state").toBool();
        m_type = clientModel->record(0).value("type").toBool();
        m_birthday = clientModel->record(0).value("birthday").toString();
        m_memorial = clientModel->record(0).value("memorial").toString();
        m_notes = clientModel->record(0).value("notes").toString();
        if(!m_notes.isEmpty())
            showNotification(m_notes, QMessageBox::Information);
        initBinaryOptions(clientModel);
        if(m_options&BinaryOption::BalanceEnabled)
            createBalanceObj();
//        m_photo = ;
        m_photoId = clientModel->record(0).value("photo_id").toInt();
        m_visitSource = clientModel->record(0).value("visit_source").toInt();
        m_INN = clientModel->record(0).value("INN").toString();
        m_KPP = clientModel->record(0).value("KPP").toString();
        m_OGRN = clientModel->record(0).value("OGRN").toString();
        m_webPassword = clientModel->record(0).value("web_password").toString();
        m_urName = clientModel->record(0).value("ur_name").toString();
        m_email = clientModel->record(0).value("email").toString();
        m_icq = clientModel->record(0).value("icq").toString();
        m_skype = clientModel->record(0).value("skype").toString();
        m_viber = clientModel->record(0).value("viber").toString();
        m_telegram = clientModel->record(0).value("telegram").toString();
        m_site = clientModel->record(0).value("site").toString();
        m_whatsapp = clientModel->record(0).value("whatsapp").toString();
        m_agentName = clientModel->record(0).value("agent_name").toString();
        m_agentSurname = clientModel->record(0).value("agent_surname").toString();
        m_agentPatronymic = clientModel->record(0).value("agent_patronymic").toString();
        m_agentPhone = clientModel->record(0).value("agent_phone").toString();
        m_agentPhoneClean = clientModel->record(0).value("agent_phone_clean").toString();
        m_agent2Name = clientModel->record(0).value("agent2_name").toString();
        m_agent2Surname = clientModel->record(0).value("agent2_surname").toString();
        m_agent2Patronymic = clientModel->record(0).value("agent2_patronymic").toString();
        m_agent2Phone = clientModel->record(0).value("agent2_phone").toString();
        m_agent2PhoneClean = clientModel->record(0).value("agent2_phone_clean").toString();
        m_priceColumn = clientModel->record(0).value("price_col").toInt();
        m_repairs = clientModel->record(0).value("repairs").toInt();
        m_purchases = clientModel->record(0).value("purchases").toInt();
        m_token = clientModel->record(0).value("token").toString();
        i_createdUtc = clientModel->record(0).value("created").toDateTime();
    }

    delete clientModel;
}

void SClientModel::initBinaryOptions(QSqlQueryModel *clientModel)
{
    m_options = 0;
    for(int i=0; i < clientBinaryProperties->rowCount(); i++)
    {
        if(clientBinaryProperties->index(i, 1).data().toInt() == BinaryOption::Archived)
            continue;
        m_options |= clientModel->record(0).value(clientBinaryProperties->index(i, 2).data().toString()).toBool()?clientBinaryProperties->index(i, 1).data().toInt():0;
    }
    m_options |= clientModel->record(0).value("state").toBool()?0:1;
}

void SClientModel::initDemo()
{
    i_id = 123;
    m_firstName = "Андрей";
    m_lastName = "Андреев";
    m_patronymicName = "Андреевич";
    m_address = "пр. Дружбы народов, д. 5";
    m_postIndex = "123000";
    m_passportNum = "111111";
    m_passportIssuedDate = QDate::currentDate().toString("dd.MM.yyyy");
    m_passportIssuedBy = "АБ ВГД Центрального района";
    m_state = 1;
    m_type = 0;
    m_birthday = QDate::currentDate().toString("dd.MM.yyyy");
    m_notes = "делать скидки на ремонт";
    m_options = BinaryOption::Regular;
    m_visitSource = clientAdTypesList->index(0, 1).data().toInt();
    m_INN = "1234567890";
    m_KPP = "4567";
    m_OGRN = "123789";
    m_webPassword = "FC9Y76U3";
    m_urName = "";
    m_email = "admin@example.com";
    m_skype = "user_skype";
    m_site = "example.com";
    m_balance = 999999;
    m_priceColumn = priceColModel->index(0, 1).data().toInt();
    m_repairs = 10;
    m_purchases = 20;
    i_createdUtc = QDateTime::currentDateTimeUtc();
}

void SClientModel::setEmployeeId(const int id)
{
    i_valuesMap.insert("employee", id);
}

void SClientModel::clear()
{
    i_id = 0;
    i_valuesMap.clear();
    i_valuesMap.insert("creator", userDbData->value("id"));
    i_valuesMap.insert("notes", "");
    m_phones->reset();
}

void SClientModel::setEditStrategy(EditStrategy strategy)
{
    m_editStrategy = strategy;
}

bool SClientModel::isStandAlone()
{
    return m_standAlone;
}

int SClientModel::id()
{
    return i_id;
}

void SClientModel::setId(const int id)
{
    i_id = id;
}

QString SClientModel::firstName()
{
    return m_firstName;
}

void SClientModel::setFirstName(const QString &text)
{
    i_valuesMap.insert("name", text);
}

QString SClientModel::lastName()
{
    return m_lastName;
}

void SClientModel::setLastName(const QString &text)
{
    i_valuesMap.insert("surname", text);
}

QString SClientModel::patronymicName()
{
    return m_patronymicName;
}

void SClientModel::setPatronymicName(const QString &text)
{
    i_valuesMap.insert("patronymic", text);
}

QString SClientModel::fullLongName()
{
    QString ret;
    if(m_type)
    {
        if(m_firstName.isEmpty())
            ret = m_urName;
        else
            ret = m_firstName;
    }
    else
    {
        ret = m_lastName;
        if(!m_firstName.isEmpty())
            ret.append(" " + m_firstName);
        if(!m_patronymicName.isEmpty())
            ret.append(" " + m_patronymicName);
    }

    return ret;
}

QString SClientModel::fullShortName()
{
    QString ret;
    if(m_type)
    {
        return fullLongName();
    }
    else
    {
        ret = m_lastName;
        if(!m_firstName.isEmpty())
            ret.append(" ").append(m_firstName.front()).append(".");
        if(!m_patronymicName.isEmpty())
            ret.append(" ").append(m_patronymicName.front()).append(".");
    }

    return ret;
}

SPhonesModel* SClientModel::phones()
{
    return m_phones;
}

void SClientModel::addPhone(const QString &number, int comboBoxMaskIndex, int messengers)
{
    SPhoneModel *phone = new SPhoneModel();
    phone->setPhone(number);
    phone->setMask(comboBoxMaskIndex);
    if(m_phones->isEmpty())
        phone->setPrimary();
    phone->setMessengers(messengers);
    m_phones->add(phone);
}

QString SClientModel::address()
{
    return m_address;
}

void SClientModel::setAddress(const QString &text)
{
    i_valuesMap.insert("address", text);
}

QString SClientModel::postIndex()
{
    return m_postIndex;
}

void SClientModel::setPostIndex(const QString &text)
{
    i_valuesMap.insert("post_index", text);
}

QString SClientModel::passportNum()
{
    return m_passportNum;
}

void SClientModel::setPassportNum(const QString &text)
{
    i_valuesMap.insert("passport_num", text);
}

QString SClientModel::passportIssuedDate()
{
    return m_passportIssuedDate;
}

void SClientModel::setPassportIssuedDate(const QString &text)
{
    i_valuesMap.insert("passport_date", text);
}

QString SClientModel::passportIssuedBy()
{
    return m_passportIssuedBy;
}

void SClientModel::setPassportIssuedBy(const QString &text)
{
    i_valuesMap.insert("passport_organ", text);
}

bool SClientModel::state()
{
    return m_state;
}

bool SClientModel::type()
{
    return m_type;
}

void SClientModel::setType(const int type)
{
    i_valuesMap.insert("type", type);
}

QString SClientModel::birthday()
{
    return m_birthday;
}

void SClientModel::setBirthday(const QString &text)
{
    i_valuesMap.insert("birthday", text);
}

QString SClientModel::memorial()
{
    return m_memorial;
}

QString SClientModel::notes()
{
    return m_notes;
}

int SClientModel::options()
{
    return m_options;
}

/*  Список свойств клиента, которые необходимо отображать в карточке ремонта
 *  или в графе с данными клиента при приёме в ремонт (в QListWidget)
*/
QStringList SClientModel::optionsList(bool shortForm)
{
    int col = 0;
    bool skip;
    if(shortForm)
        col = 3;
    QStringList list;
    for(int i = 0; i < clientBinaryProperties->rowCount(); i++)
    {
        switch(m_options & (1<<i))
        {
//            case BinaryOption::Company:   // type()
//            case BinaryOption::BalanceEnabled:    // balanceEnabled()
//            case BinaryOption::Archived:  //  state()
            case BinaryOption::Supplier:
            case BinaryOption::SaleOrReturn:
            case BinaryOption::Regular:
            case BinaryOption::Broker:
            case BinaryOption::IgnoreCalls:
            case BinaryOption::PreferCashless:
            case BinaryOption::TakeLong:
            case BinaryOption::Bad: skip = 0; break;
            default: skip = 1;
        }
        if(!skip)
            list << clientBinaryProperties->index(i, col).data().toString();
    }
    return list;
}

bool SClientModel::isSupplier()
{
    return m_options & BinaryOption::Supplier;
}

bool SClientModel::isGivesItemsForSale()
{
    return m_options & BinaryOption::SaleOrReturn;
}

bool SClientModel::isArchived()
{
    return !state();
}

bool SClientModel::isRegular()
{
    return m_options & BinaryOption::Regular;
}

bool SClientModel::isBroker()
{
    return m_options & BinaryOption::Broker;
}

bool SClientModel::isIgnoreCalls()
{
    return m_options & BinaryOption::IgnoreCalls;
}

bool SClientModel::isPreferCashless()
{
    return m_options & BinaryOption::PreferCashless;
}

bool SClientModel::isTakeLong()
{
    return m_options & BinaryOption::TakeLong;
}

bool SClientModel::isBad()
{
    return m_options & BinaryOption::Bad;
}

int SClientModel::adType()
{
    return m_visitSource;
}

void SClientModel::setAdType(const int id)
{
    i_valuesMap.insert("visit_source", id);
}

void SClientModel::setAdTypeIndex(const int index)
{
    if(index == -1)
        i_valuesMap.insert("visit_source", QVariant());
    else
        setAdType(clientAdTypesList->databaseIDByRow(index));
}

int SClientModel::adTypeIndex()
{
    return clientAdTypesList->rowByDatabaseID(m_visitSource, 1);
}

QString SClientModel::adTypeStr()
{
    return clientAdTypesList->getDisplayRole(adType(), "id");
}

QByteArray* SClientModel::photo()
{
    return nullptr;
}

QString SClientModel::INN()
{
    return m_INN;
}

QString SClientModel::KPP()
{
    return m_KPP;
}

QString SClientModel::OGRN()
{
    return m_OGRN;
}

QString SClientModel::web_password()
{
    return m_webPassword;
}

QString SClientModel::urName()
{
    return m_urName;
}

void SClientModel::setUrName(const QString &name)
{
    i_valuesMap.insert("ur_name", name);
}

QString SClientModel::email()
{
    return m_email;
}

QString SClientModel::icq()
{
    return m_icq;
}

QString SClientModel::skype()
{
    return m_skype;
}

QString SClientModel::viber()
{
    return m_viber;
}

QString SClientModel::telegram()
{
    return m_telegram;
}

QString SClientModel::site()
{
    return m_site;
}

QString SClientModel::whatsapp()
{
    return m_whatsapp;
}

QString SClientModel::agentName()
{
    return m_agentName;
}

QString SClientModel::agentSurname()
{
    return m_agentSurname;
}

QString SClientModel::agentPatronymic()
{
    return m_agentPatronymic;
}

QString SClientModel::agentPhone()
{
    return m_agentPhone;
}

QString SClientModel::agentPhoneClean()
{
    return m_agentPhoneClean;
}

QString SClientModel::agent2Name()
{
    return m_agent2Name;
}

QString SClientModel::agent2Surname()
{
    return m_agent2Surname;
}

QString SClientModel::agent2Patronymic()
{
    return m_agent2Patronymic;
}

QString SClientModel::agent2Phone()
{
    return m_agent2Phone;
}

QString SClientModel::agent2PhoneClean()
{
    return m_agent2PhoneClean;
}

bool SClientModel::balanceEnabled()
{
    return m_options&BalanceEnabled;
}

/*  Включение/отключение баланса клиента
 *  Возвращает 0 в случае ошибки
*/
bool SClientModel::setBalanceEnabled(bool state)
{
    QSqlQuery *query;

    if( state != (bool)(m_options&BalanceEnabled) )
    {

        if(state)
        {
            appendLogText(tr("Включен баланс клиента"));
            createBalanceObj();
            m_options |= BalanceEnabled;
        }
        else
        {
            if(m_balance != 0)
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
                m_options &= ~BalanceEnabled;
            }
        }
        i_valuesMap.insert("balance_enable", state);

        query = new QSqlQuery(QSqlDatabase::database("connThird"));
        QUERY_EXEC(query,i_nErr)(QUERY_BEGIN);
        commit();
        QUERY_COMMIT_ROLLBACK(query,i_nErr);
        delete query;
    }
    return i_nErr;
}

double SClientModel::balance()
{
    return m_balance;
}

QString SClientModel::balanceStr()
{
    return sysLocale.toCurrencyString(balance());   // TODO: код валюты для мультивалютных балансов/касс
}

void SClientModel::createBalanceObj()
{
    balanceLog = new SBalanceLogRecordModel(i_id);
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_BALANCE(i_id));
    query->first();
    if(query->isValid())
        m_balance = query->value(0).toDouble();
    delete query;
}

SBalanceLogRecordModel *SClientModel::balanceObj()
{
    return balanceLog;
}

void SClientModel::deleteBalanceObj()
{
    if(balanceLog)
        delete balanceLog;
}

bool SClientModel::updateBalance(const double amount, const QString &text)
{
    if(amount == 0)
        return 1;

    if(isNew())   // TODO: проверка включен ли баланс у клиента
        throw 3;

    if(amount < 0 && m_balance < -amount)
    {
        QMessageBox resBtn( QMessageBox::Question, "SNAP CRM",
                            tr("На балансе недостаточно средств, продолжить?"),
                            QMessageBox::No | QMessageBox::Yes);
        QAbstractButton *noButton = resBtn.button(QMessageBox::No);
        resBtn.setDefaultButton(QMessageBox::No);
        resBtn.setWindowModality(Qt::ApplicationModal);
        resBtn.exec();
        if (resBtn.clickedButton() == noButton)
            throw 4;
    }

    balanceLog->setText(text);
    balanceLog->setClient(i_id);
    i_nErr = balanceLog->commit(amount);
    double newAmount = m_balance + amount;

    QUERY_EXEC(i_query,i_nErr)(QUERY_UPDATE_BALANCE(i_id, newAmount));
    QUERY_EXEC(i_query,i_nErr)(QUERY_VRFY_BALANCE(i_id));

    if(!i_nErr)
    {
        throw 1;
    }

    QUERY_EXEC_VRFY(i_query,nIntegrityErr);

    if(!nIntegrityErr)
    {
        showNotification(tr("Ошибка целостности данных баланса (id = %1)").arg(i_id), QMessageBox::Critical);
        throw 2;
    }

    m_balance = i_query->value("balance").toDouble();
    return i_nErr;
}

bool SClientModel::updateBalance(const double amount, const QString &text, const SBalanceLogRecordModel::RoyaltyReason reason, const int reason_id)
{
    switch(reason)
    {
        case SBalanceLogRecordModel::RoyaltyReason::Repair: balanceLog->setRepair(reason_id); break;
        case SBalanceLogRecordModel::RoyaltyReason::Document: balanceLog->setDocumentId(reason_id); break;
        case SBalanceLogRecordModel::RoyaltyReason::CashOrder: balanceLog->setCashOrderId(reason_id); break;
    }
    updateBalance(amount, text);

    return i_nErr;
}

int SClientModel::priceColumn()
{
    return m_priceColumn;
}

int SClientModel::priceColumnIndex()
{
    if(!m_priceColumn)  // на случай ошибок в БД, по умолчанию тип цен — Розница
        return priceColModel->rowByDatabaseID(2, 1);

    return priceColModel->rowByDatabaseID(m_priceColumn, 1);
}

QString SClientModel::priceColumnStr()
{
    return priceColModel->getDisplayRole(priceColumn(), "id");
}

int SClientModel::repairs()
{
    return m_repairs;
}

void SClientModel::setRepairs(const int val)
{
    i_valuesMap.insert("repairs", val);
}

bool SClientModel::updateRepairs(const int val)
{
    if(i_id == 0)
        return 1;

    setRepairs(repairs()+val);
    commit();

    if(!i_nErr)
        throw 1;

    m_repairs += val;
    return i_nErr;
}

int SClientModel::purchases()
{
    return m_purchases;
}

bool SClientModel::updatePurchases(int items)
{
    if(i_id == 0)
        return 1;

    // в АСЦ с кол-вом покупок то ли глюк, то ли странная задумка: в таблице клиентов отображается кол-во РН, а в самой карте клиента кол-во записей в таблице store_sales
    // TODO: разобраться с этим
    QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_CLIENT_PURCHASES(i_id, items));

    if(!i_nErr)
        throw 1;

    return i_nErr;
}

QString SClientModel::token()
{
    return m_token;
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

    msgBox.setWindowTitle(fullShortName());
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

/* Добавление текста журнала
 * При редактировании карточки клиента, пользователь может изменить несколько свойств
 * за раз и при использовании стратегии OnManualSubmit обо всех изменениях нужно
 * сделать запись в журнал.
 */
void SClientModel::appendLogText(const QString &text)
{
    SComRecord::appendLogText(text);
}

bool SClientModel::commit()
{
    if(i_id)
    {
        if(!update())
            throw 1;
    }
    else
    {
        i_valuesMap.insert("created", QDateTime::currentDateTime());
        i_valuesMap.insert("web_password", genWebPass());
        if(!insert())
            throw 1;
    }

    i_logRecord->setClient(i_id);
    if(!commitLogs())
        throw 1;

    m_phones->setClient(i_id);
    if(!m_phones->commit())
        throw 1;    // исключение генерируется в SPhonesModel::commit(), поэтому это выражение не выполнится никогда

    return i_nErr;
}

bool SClientModel::balanceEnough(const QString summ)
{
    if( sysLocale.toDouble(summ) <= m_balance )
        return 1;

    QMessageBox msgBox;

    msgBox.setWindowTitle("SNAP CRM");
    msgBox.setText(tr("На балансе клиента недостаточно средств"));
    msgBox.setInformativeText("продолжить?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
       return 1;
    }

    return 0;
}

bool SClientModel::integrityStatus()
{
    return nIntegrityErr;
}

SBalanceLogRecordModel::SBalanceLogRecordModel(QObject *parent):
    SComRecord(parent)
{
    i_obligatoryFields << "client" << "summ" << "direction" << "reason" << "created" << "office" << "uid";
    i_tableName = "balance";
    i_logRecord->setType(SLogRecordModel::Client);
}

SBalanceLogRecordModel::SBalanceLogRecordModel(int client, QObject *parent):
    SBalanceLogRecordModel(parent)
{
    setClient(client);
}

SBalanceLogRecordModel::~SBalanceLogRecordModel()
{

}

int SBalanceLogRecordModel::id()
{
    return i_id;
}

void SBalanceLogRecordModel::setClient(int id)
{
    m_client = id;
    i_logRecord->setClient(id);
    i_valuesMap.insert("client", id);
}

void SBalanceLogRecordModel::setText(const QString &text)
{
    i_logRecord->setText(text);
    i_valuesMap.insert("reason", text);
}

void SBalanceLogRecordModel::setDirection(double amount)
{
    if(amount > 0)
        i_valuesMap.insert("direction", 1);
    else
        i_valuesMap.insert("direction", 0);
}

void SBalanceLogRecordModel::setRepair(const int id)
{
    i_logRecord->setRepairId(id);
}

void SBalanceLogRecordModel::setDocumentId(const int id)
{
    i_logRecord->setDocumentId(id);
}

void SBalanceLogRecordModel::setCashOrderId(const int id)
{
    i_logRecord->setCashOrderId(id);
}

bool SBalanceLogRecordModel::commit(const double amount)
{
    setDirection(amount);
    i_valuesMap.insert("uid", userDbData->value("id"));
    i_valuesMap.insert("office", userDbData->value("current_office"));
    i_valuesMap.insert("summ", amount);
    if(!i_valuesMap.contains("created"))
        i_valuesMap.insert("created", QDateTime::currentDateTime());

    insert();

    i_logRecord->commit();
    return i_nErr;
}

bool SBalanceLogRecordModel::commit(const double amount, const QString &text)
{
    setText(text);
    commit(amount);
    return i_nErr;
}
