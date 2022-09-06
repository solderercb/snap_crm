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
    if(balanceLog)
        delete balanceLog;
}

bool SClientModel::isNew()
{
    if(i_id)
        return false;

    return true;
}

void SClientModel::load(int id)
{
    i_id = id;

    i_valuesMap.clear();
    clientModel = new SSqlQueryModel(this);
    m_phones->load(i_id);

    i_logRecord->setClient(i_id);

    clientModel->setQuery(QUERY_SEL_CLIENT(i_id), QSqlDatabase::database("connMain"));

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
    showNotification();
    m_options = (clientModel->record(0).value("is_regular").toInt()?Option::Regular:0) | \
                (clientModel->record(0).value("is_dealer").toInt()?Option::Dealer:0) | \
                (clientModel->record(0).value("balance_enable").toInt()?Option::BalanceEnabled:0) | \
                (clientModel->record(0).value("prefer_cashless").toInt()?Option::PreferCashless:0) | \
                (clientModel->record(0).value("take_long").toInt()?Option::TakeLong:0) | \
                (clientModel->record(0).value("ignore_calls").toInt()?Option::IgnoreCalls:0) | \
                (clientModel->record(0).value("is_bad").toInt()?Option::Bad:0) | \
                (clientModel->record(0).value("is_realizator").toInt()?Option::Realizator:0) | \
                (clientModel->record(0).value("is_agent").toInt()?Option::Agent:0);
    if(m_options&Option::BalanceEnabled)
        createBalanceObj();
//    m_photo = ;
    m_photo_id = clientModel->record(0).value("photo_id").toInt();
    m_visitSource = clientModel->record(0).value("visit_source").toInt();
    m_INN = clientModel->record(0).value("INN").toString();
    m_KPP = clientModel->record(0).value("KPP").toString();
    m_OGRN = clientModel->record(0).value("OGRN").toString();
    m_web_password = clientModel->record(0).value("web_password").toString();
    m_ur_name = clientModel->record(0).value("ur_name").toString();
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
    m_balance = clientModel->record(0).value("balance").toFloat();
    m_priceColumn = clientModel->record(0).value("price_col").toInt();
    m_repairs = clientModel->record(0).value("repairs").toInt();
    m_purchases = clientModel->record(0).value("purchases").toInt();
    m_token = clientModel->record(0).value("token").toString();
    i_createdUtc = clientModel->record(0).value("created").toDateTime();

    delete clientModel;
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
    return QString();
}

QString SClientModel::fullShortName()
{
    return QString();
}

SPhonesModel* SClientModel::phones()
{
    return m_phones;
}

bool SClientModel::addPhone(const QString &number, int comboBoxMaskIndex, int messengers)
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

int SClientModel::adType()
{
    return m_visitSource;
}

void SClientModel::setAdType(const int id)
{
    i_valuesMap.insert("visit_source", id);
}

int SClientModel::adTypeIndex()
{
    return clientAdTypesList->rowByDatabaseID(m_visitSource, 1);
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
    return m_web_password;
}

QString SClientModel::ur_name()
{
    return m_ur_name;
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

float SClientModel::balance()
{
    return m_balance;
}

void SClientModel::createBalanceObj()
{
    balanceLog = new SBalanceLogRecordModel(i_id);
}

bool SClientModel::updateBalance(const float amount, const QString &text)
{
    if(isNew())   // TODO: проверка включен ли баланс у клиента
        return 1;

    balanceLog->setText(text);
    i_nErr = balanceLog->commit(amount);

    QUERY_EXEC(i_query,i_nErr)(QUERY_UPDATE_BALANCE(i_id, amount));
    QUERY_EXEC(i_query,i_nErr)(QUERY_VRFY_BALANCE(i_id));

    if(!i_nErr)
        throw 1;

    QUERY_EXEC_VRFY(i_query,nIntegrityErr);

    if(!nIntegrityErr)
    {
        throw 2;
    }

    return i_nErr;
}

bool SClientModel::updateBalance(const float amount, const QString &text, const int doc_id)
{
    balanceLog->setDocumentId(doc_id);
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

int SClientModel::repairs()
{
    return m_repairs;
}

bool SClientModel::updateRepairs()
{
    if(i_id == 0)
        return 1;

    QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_CLIENT_REPAIRS(i_id));

    if(!i_nErr)
        throw 1;

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

void SClientModel::showNotification()
{
    if(!m_notes.isEmpty())
    {
        QMessageBox msgBox;

        msgBox.setText(m_notes);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
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
    if( sysLocale.toFloat(summ) <= m_balance )
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
    i_valuesMap.insert("uid", userDbData->value("id"));
    i_valuesMap.insert("office", userDbData->value("current_office"));
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

void SBalanceLogRecordModel::setDirection(float amount)
{
    if(amount > 0)
        i_valuesMap.insert("direction", 1);
    else
        i_valuesMap.insert("direction", 0);
}

void SBalanceLogRecordModel::setDocumentId(int id)
{
    i_logRecord->setDocumentId(id);
}

bool SBalanceLogRecordModel::commit(const float amount)
{
    setDirection(amount);
    i_valuesMap.insert("summ", amount);
    i_valuesMap.insert("created", QDateTime::currentDateTime());

    insert();

    i_logRecord->commit();
    return i_nErr;
}

bool SBalanceLogRecordModel::commit(const float amount, const QString &text)
{
    setText(text);
    commit(amount);
    return i_nErr;
}
