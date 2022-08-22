#include "scashregistermodel.h"

SCashRegisterModel::SCashRegisterModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "created" << "type" << "summa" << "user" << "company" << "office" << "notes";
    tableName = "cash_orders";
}

SCashRegisterModel::SCashRegisterModel(int systemId, QObject *parent) :
    SCashRegisterModel(parent)
{
    setSystemId(systemId);
}

SCashRegisterModel::~SCashRegisterModel()
{
}

void SCashRegisterModel::setId(int id)
{
    i_id = id;
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
    m_amount = order->record(0).value("summa").toFloat();
    m_amount_str = order->record(0).value("summa_str").toString();
    m_invoice = order->record(0).value("invoice").toInt();
    m_client = order->record(0).value("client").toInt();
    m_toUser = order->record(0).value("to_user").toInt();
    m_user = order->record(0).value("user").toInt();
    m_company = order->record(0).value("company").toInt();
    m_office = order->record(0).value("office").toInt();
    m_notes = order->record(0).value("notes").toString();
    m_repair = order->record(0).value("repair").toInt();
    m_document = order->record(0).value("document").toInt();
    m_img = order->record(0).value("img").toByteArray();
    m_systemId = order->record(0).value("payment_system").toInt();
    m_cardFee = order->record(0).value("card_fee").toFloat();
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

/*  Создание новой записи о кассовой операции в БД
 *  возвращает 0 в случае успеха
 */
bool SCashRegisterModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
        bool nIntegrityErr = 1;
        QString q;

        i_valuesMap.insert("user", userDbData->value("id"));
        i_valuesMap.insert("company", userDbData->value("company"));
        i_valuesMap.insert("office", userDbData->value("current_office"));
        i_valuesMap.insert("created", QDateTime::currentDateTime());

        fieldsVerifyFormatter();    // вызов этого метода должен быть до вызова метода insert()

        insert();

        // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
        q = "SELECT IF(\n  " + fields_verify.join(" AND\n  ") + "\n, 21930, 0)\n"\
                                                                "FROM `cash_orders`\n"\
                                                                "WHERE `id` = " + QString::number(i_id) + ";";
        QUERY_EXEC(query,i_nDBErr)(q);
        QUERY_EXEC_VRFY(query,nIntegrityErr);

        delete query;

        if(!nIntegrityErr)
            throw 2;
    }

    if(!i_nDBErr)
        throw 1;

    i_logRecord->commit();
    // TODO: Признак предмета расчета

    return i_nDBErr;
}

bool SCashRegisterModel::commit(float amount)
{
    i_valuesMap.insert("summa", amount);
    i_valuesMap.insert("summa_str", amountToWords(amount));
    return commit();
}

void SCashRegisterModel::fieldsVerifyFormatter()
{
    QMap<QString, QVariant>::ConstIterator i;

    fields_verify.clear();
    for (i = i_valuesMap.constBegin(); i != i_valuesMap.constEnd(); ++i)
    {
        fields_verify.append("IFNULL(`" + i.key() + "`,'NULL') = IFNULL(" + fieldValueHandler(i.value()) + ",'NULL')");
    }

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

    i_valuesMap.insert("client", id);
//    i_logRecord->setClient(id);   // При просмотре записей журнала, касающихся клиента, записи о кассовых операциях отображать не нужно
}

void SCashRegisterModel::unsetClient()
{
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
void SCashRegisterModel::setCreated(QDateTime timestamp)
{
    i_valuesMap.insert("created", timestamp);
}

int SCashRegisterModel::documentId()
{
    return m_document;
}

void SCashRegisterModel::setDocumentId(int id)
{
    i_valuesMap.insert("document", id);
    i_logRecord->setType(SLogRecordModel::Doc);
    i_logRecord->setDocumentId(id);
}

int SCashRegisterModel::repairId()
{
    return m_repair;
}

void SCashRegisterModel::setRepairId(int id)
{
    i_valuesMap.insert("repair", id);
    i_logRecord->setType(SLogRecordModel::Repair);
    i_logRecord->setRepairId(id);
}

/* Установка текста записи в журнале
 * В случае ПН или РН его нельзя задать автоматически
 */
void SCashRegisterModel::setLogText(const QString &text)
{
    i_logRecord->setText(text);
}

int SCashRegisterModel::operationType()
{
    return m_operationType;
}

void SCashRegisterModel::setOperationType(int type)
{
    i_valuesMap.insert("type", type);
}

