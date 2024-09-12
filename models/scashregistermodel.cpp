#include "scashregistermodel.h"

SCashRegisterModel::SCashRegisterModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "created" << "type" << "summa" << "user" << "company" << "office" << "notes";
    i_tableName = "cash_orders";
    i_idColumnName = "id";
    m_amount_str = sysLocale.toCurrencyString(m_amount);
    m_amount_words = amountToWords(m_amount);
}

SCashRegisterModel::SCashRegisterModel(int systemId, QObject *parent) :
    SCashRegisterModel(parent)
{
    setSystemId(systemId);
}

SCashRegisterModel::~SCashRegisterModel()
{
}

int SCashRegisterModel::id()
{
    return i_id;
}

void SCashRegisterModel::setId(const int id)
{
    i_id = id;

    // TODO: этот метод предназначен для сброса id в нуль; при этом, вероятно, нужно очистить кэш, а также сборосить в умолчания все переменные
}

void SCashRegisterModel::load()
{
    QSqlQueryModel *order = new QSqlQueryModel();
    order->setQuery(QUERY_SEL_CASH_ORDER(i_id), QSqlDatabase::database("connMain"));
    if(!order->rowCount())
        return;

    i_valuesMap.clear();
    i_createdUtc = order->record(0).value("created").toDateTime();
    m_type = order->record(0).value("type").toInt();
    m_amount = order->record(0).value("summa").toDouble();
    m_amount_str = order->record(0).value("summa_str").toString();
    m_invoice = order->record(0).value("invoice").toInt();
    m_client = order->record(0).value("client").toInt();
    m_toUser = order->record(0).value("to_user").toInt();
    m_user = order->record(0).value("user").toInt();
    m_company = order->record(0).value("company").toInt();
    m_office = order->record(0).value("office").toInt();
    m_reason = order->record(0).value("notes").toString();
    m_repair = order->record(0).value("repair").toInt();
    m_document = order->record(0).value("document").toInt();
    m_img = order->record(0).value("img").toInt();
    m_systemId = order->record(0).value("payment_system").toInt();
    m_cardFee = order->record(0).value("card_fee").toDouble();
    m_isBackdate = order->record(0).value("is_backdate").toBool();
    m_cardInfo = order->record(0).value("card_info").toInt();
    m_customerEmail = order->record(0).value("customer_email").toString();
    m_fdn = order->record(0).value("fdn").toInt();
    m_paymentItemAttribute = order->record(0).value("payment_item_sign").toInt();

    delete order;
}

void SCashRegisterModel::load(int id)
{
    setId(id);
    load();
}

// демо-данные (например, для отчетов)
void SCashRegisterModel::initDemo()
{
    i_id = 123456;
    i_createdUtc = QDateTime::currentDateTimeUtc();
    m_type = RecptSimple;
    m_amount = 1024;
    m_amount_str = sysLocale.toCurrencyString(m_amount);
    m_amount_words = amountToWords(m_amount);
    m_invoice = 0;
    m_client = 0;
    m_toUser = usersModel->index(0, 1).data().toInt();
    m_user = usersModel->index(0, 1).data().toInt();
    m_company = 1;
    m_office = 1;
    m_reason = tr("Поступление денег в размере %1").arg(m_amount_str);
    m_repair = 0;
    m_document = 0;
    m_img = 0;
    m_systemId = paymentSystemsModel->index(0, 1).data().toInt();
    m_cardFee = 0;
    m_isBackdate = 0;
    m_cardInfo = 0;
    m_fdn = 0;
    m_paymentItemAttribute = 0;
}

/*  Создание новой записи о кассовой операции в БД
 *  возвращает 1 в случае успеха
 */
bool SCashRegisterModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        if(m_amount == 0)
            return 1;

        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
        bool nIntegrityErr = 1;
        QString q;

        i_valuesMap.insert("user", userDbData->id);
        if(!i_valuesMap.contains("company"))
            i_valuesMap.insert("company", userDbData->company);
        if(!i_valuesMap.contains("office"))
            i_valuesMap.insert("office", userDbData->currentOffice);
        if(!i_valuesMap.contains("created"))
            setCreated(QDateTime::currentDateTime());

        fieldsVerifyFormatter();    // вызов этого метода должен быть до вызова метода insert()

        insert();

        // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
        q = "SELECT IF(\n  " + fields_verify.join(" AND\n  ") + "\n, 21930, 0)\n"\
                                                                "FROM `cash_orders`\n"\
                                                                "WHERE `id` = " + QString::number(i_id) + ";";
        QUERY_EXEC(query,i_nErr)(q);
        QUERY_EXEC_VRFY(query,nIntegrityErr);

        delete query;

        if(!nIntegrityErr)
            throw Global::ThrowType::IntegrityError;
    }

    if(!i_nErr)
        throw Global::ThrowType::QueryError;

    if(!m_skipLogRecording)
        i_logRecord->commit();

    // TODO: Признак предмета расчета

    return i_nErr;
}

bool SCashRegisterModel::commit(double amount)
{
    setAmount(amount);
    return commit();
}

void SCashRegisterModel::fieldsVerifyFormatter()
{
    QMap<QString, QVariant>::ConstIterator i;

    fields_verify.clear();
    for (i = i_valuesMap.constBegin(); i != i_valuesMap.constEnd(); ++i)
    {
        if(i.key() == "type" || i.key() == "notes")
            continue; // верификацию этих полей пропускаем, т. к. костыли для АСЦ в виде триггеров могут их изменить и сессия завершится Rollback'ом
        fields_verify.append("IFNULL(`" + i.key() + "`,'NULL') = IFNULL(" + fieldValueHandler(i.value()) + ",'NULL')");
    }

}

QString SCashRegisterModel::customerEmail()
{
    return m_customerEmail;
}

void SCashRegisterModel::setCustomerEmail(const QString &email)
{
    i_valuesMap.insert("customer_email", email);
}

int SCashRegisterModel::client()
{
    return m_client;
}

void SCashRegisterModel::setClient(int id)
{
    if(id == 0)
    {
        unsetClient();
        return;
    }

    m_client = id;
    i_valuesMap.insert("client", m_client);

    // При просмотре записей журнала, касающихся клиента, записи о кассовых операциях отображать не нужно (для этого есть отдельная страница в карточке клиента)
//    i_logRecord->setClient(id);
}

void SCashRegisterModel::unsetClient()
{
    m_client = 0;
    i_valuesMap.insert("client", QVariant());
//    i_logRecord->unsetClient();
}

int SCashRegisterModel::systemId()
{
    return m_systemId;
}

void SCashRegisterModel::setSystemId(int id)
{
    m_systemId = id;
    i_valuesMap.insert("payment_system", id);
}

QString SCashRegisterModel::reason()
{
    return m_reason;
}

void SCashRegisterModel::setReason(const QString &text)
{
    m_reason = text;
    i_valuesMap.insert("notes", text);
    if(!m_skipLogRecording)
        i_logRecord->setText(text);
}

/*  Код валюты
 *  (это задел на бивалютную или мультиваллютную систему)
 */
int SCashRegisterModel::currency()
{
    return m_currency;
}

/*  Установка кода валюты
 *  (это задел на бивалютную или мультиваллютную систему)
 */
void SCashRegisterModel::setCurrency(int id)
{
    i_valuesMap.insert("currency", id);
    i_logRecord->setCurrencyId(id);
}

/*  Для кассовых операций задним числом
 *  timestamp - локальное время (не UTC)
 */
void SCashRegisterModel::setCreated(const QDateTime &timestamp)
{
    SComRecord::setCreated(timestamp);
    if(timestamp.date() != QDate::currentDate())
        i_valuesMap.insert("is_backdate", 1);
}

void SCashRegisterModel::setCreatedDate(const QDate &date)
{
    QDateTime timestamp = i_valuesMap.value("created", QDateTime::currentDateTime()).toDateTime();
    timestamp.setDate(date);
    setCreated(timestamp);
}

void SCashRegisterModel::setCreatedTime(const QTime &time)
{
    QDateTime timestamp = i_valuesMap.value("created", QDateTime::currentDateTime()).toDateTime();
    timestamp.setTime(time);
    setCreated(timestamp);
}

int SCashRegisterModel::documentId()
{
    return m_document;
}

void SCashRegisterModel::setDocumentId(int id)
{
    m_document = id;
    i_valuesMap.insert("document", m_document);
    i_logRecord->setType(SLogRecordModel::Doc);
    i_logRecord->setDocumentId(m_document);
}

int SCashRegisterModel::repairId()
{
    return m_repair;
}

void SCashRegisterModel::setRepairId(int id)
{
    m_repair = id;
    i_valuesMap.insert("repair", m_repair);
    i_logRecord->setType(SLogRecordModel::Repair);
    i_logRecord->setRepairId(m_repair);
}

int SCashRegisterModel::invoiceId()
{
    return m_invoice;
}

void SCashRegisterModel::setInvoiceId(int id)
{
    m_invoice = id;
    i_valuesMap.insert("invoice", m_invoice);
//    i_logRecord->setType(SLogRecordModel::Invoice);   // В АСЦ v3.7.31.1123 не реализовано
//    i_logRecord->setInvoiceId(m_invoice);
}

/* Установка текста записи в журнале
 * В случае ПН или РН его нельзя задать автоматически
 */
void SCashRegisterModel::setLogText(const QString &text)
{
    i_logRecord->setText(text);
}

QString SCashRegisterModel::constructReason(const QString &linkedObjId)
{
    QString reason;
    switch(m_type)
    {
        case PaymentType::ExpSimple:    reason = tr("Расход денег в размере %1").arg(m_amount_str); break;
        case PaymentType::ExpInvoice:   reason = tr("Оплата приходной накладной №%1, в размере %2").arg(linkedObjId, m_amount_str); break;
        case PaymentType::ExpZ:         reason = tr("Z отчёт, выемка средств в размере %1").arg(m_amount_str); break;
        case PaymentType::ExpBalance:   reason = tr("Выдача денег в размере %1 со списанием с баланса клиента №%2").arg(m_amount_str).arg(m_client); break;  // TODO: добавить короткую форму ФИО
        case PaymentType::ExpSubsist:   reason = tr("Выплата аванса сторуднику в размере %1").arg(m_amount_str); break;
        case PaymentType::ExpSalary:    reason = tr("Выплата заработной платы сторуднику в размере %1").arg(m_amount_str); break;
        case PaymentType::ExpRepair:    reason = tr("Расход денег в размере %1 - возврат за невыполненный ремонт №2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::ExpGoods:     reason = tr("Расход денег в размере %1 - возврат за товары по РН №%2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::RecptSimple:  reason = tr("Поступление денег в размере %1").arg(m_amount_str); break;
        case PaymentType::RecptPrepayRepair: reason = tr("Предоплата за ремонт №%1 в размере %2").arg(linkedObjId, m_amount_str); break;
        case PaymentType::RecptBalance: reason = tr("Поступление денег в размере %1 с зачислением на баланс клиента №%2").arg(m_amount_str).arg(m_client); break;
        case PaymentType::RecptGoods:   reason = tr("Поступление денег в размере %1 по расходной накладной №%2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::RecptRepair:  reason = tr("Поступление денег в размере %1 в счёт выполненного(-ых) ремонта(-ов): %2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::ExpInvoiceUndo: reason = tr("Поступление средств в рамере %1. за товары в распроведённой ПН №%2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::RecptInvoice: reason = tr("Поступление денег в размере %1 по счёту №%2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::MoveCash:     reason = tr("Перемещение средств в размере %1; комплементарная операция №%2").arg(m_amount_str, linkedObjId); break;
        case PaymentType::ExpDealer:    reason = tr("Расход денег в размере %1 в счёт выплаты поставщику за товары находившиеся на реализации").arg(m_amount_str); break;
        case PaymentType::ExpRevert:    reason = tr("Возврат средств в размере %1").arg(m_amount_str); break;
    }
    if(m_type == PaymentType::AddSubCash)
    {
        if(m_amount > 0)
            reason = tr("Поступление денег в размере %1 (%2) - внесение средств в кассу").arg(m_amount_str, m_amount_words);
        else
            reason = tr("Расход денег в размере %1 (%2) - выдача средств из кассы").arg(m_amount_str, m_amount_words);
    }
    else if(m_type > PaymentType::ExpCustom)  // TODO: В АСЦ v3.7.31.1123 есть шаблоны только РКО
        reason = tr("Расход денег в размере %1, %2").arg(m_amount_str, expenditureTypesModel->reasonByDatabaseId(m_type));

    setReason(reason);

    return reason;
}

QString SCashRegisterModel::constructReason(int linkedObjId)
{
    return constructReason(QString::number(linkedObjId));
}

/*  По умолчанию каждая кассовая операция сопровождается записью в журнал
 *  Но в некоторых случаях запись в журнал может производиться в отдельном методе класса,
 *  использующего объект этого класса, что приведёт к созданию двух записей в журнале.
 *  Например, при создании ПКО предоплаты ремонта. Чтобы этого избежать нужно вызвать
 *  этот метод без параметров или с параметром state = true.
*/
void SCashRegisterModel::setSkipLogRecording(bool state)
{
    m_skipLogRecording = state;
}

void SCashRegisterModel::setOffice(const int id)
{
    m_office = id;
    i_valuesMap.insert("office", id);
}

void SCashRegisterModel::setOfficeIndex(const int index)
{
    setOffice(officesModel->databaseIDByRow(index));
}

int SCashRegisterModel::operationType()
{
    return m_type;
}

void SCashRegisterModel::setOperationType(int type)
{
    m_type = type;
    i_valuesMap.insert("type", m_type);
}

double SCashRegisterModel::amount()
{
    return m_amount;
}

double SCashRegisterModel::amountAbs()
{
    if(m_amount < 0)
        return -m_amount;
    return m_amount;
}

void SCashRegisterModel::setAmount(double amount)
{
    double l_amount = 0;

    m_amount = amount;
    l_amount = amountAbs();
    m_amount_str = sysLocale.toCurrencyString(l_amount);
    m_amount_words = amountToWords(l_amount);
    i_valuesMap.insert("summa", amount);
    setAmountStr(m_amount_words);
}

QString SCashRegisterModel::amountStr()
{
    return m_amount_str;
}

void SCashRegisterModel::setAmountStr(const QString amount)
{
    i_valuesMap.insert("summa_str", amount);
}

/*  Id сотрудника, которому выданы деньги (зарплата)
*/
int SCashRegisterModel::employee()
{
    return m_toUser;
}

void SCashRegisterModel::setEmployee(const int id)
{
    m_toUser = id;
    i_logRecord->setType(SLogRecordModel::User);
    i_logRecord->setUser(id);
    i_valuesMap.insert("to_user", id);
}

/*  Id сотрудника, который выдал деньги (зарплата)
*/
int SCashRegisterModel::user()
{
    return m_user;
}

void SCashRegisterModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

int SCashRegisterModel::company()
{
    return m_company;
}

int SCashRegisterModel::companyIndex()
{
    return companiesModel->rowByDatabaseID(m_company, "id");
}

void SCashRegisterModel::setCompany(const int id)
{
    m_company = id;
    i_valuesMap.insert("company", id);
}

void SCashRegisterModel::setCompanyIndex(const int index)
{
    setCompany(companiesModel->databaseIDByRow(index, "id"));
}

int SCashRegisterModel::office()
{
    return m_office;
}

int SCashRegisterModel::img()
{
    return m_img;
}

void SCashRegisterModel::setImg(const int img)
{
    i_valuesMap.insert("img", img);
}

double SCashRegisterModel::cardFee()
{
    return m_cardFee;
}

void SCashRegisterModel::setCardFee(const double fee)
{
    i_valuesMap.insert("card_fee", fee);
}

bool SCashRegisterModel::isBackdate()
{
    return m_isBackdate;
}

void SCashRegisterModel::setIsBackdate(const bool status)
{
    i_valuesMap.insert("is_backdate", status);
}

int SCashRegisterModel::cardInfo()
{
    return m_cardInfo;
}

void SCashRegisterModel::setCardInfo(const int info)
{
    i_valuesMap.insert("card_info", info);
}

int SCashRegisterModel::fdn()
{
    return m_fdn;
}

void SCashRegisterModel::setFdn(const int fdn)
{
    i_valuesMap.insert("fdn", fdn);
}

int SCashRegisterModel::paymentItemSign()
{
    return m_paymentItemAttribute;
}

void SCashRegisterModel::setPaymentItemSign(const int attr)
{
    i_valuesMap.insert("payment_item_sign", attr);
}

