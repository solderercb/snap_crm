#include "sdocumentmodel.h"

SDocumentModel::SDocumentModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "company" << "user" << "total" << "created" << "office" << "img1" << "img2" << "img3";
    tableName = "docs";
    i_logRecord->setType(SLogRecordModel::Doc);
}

SDocumentModel::SDocumentModel(int id, QObject *parent) : SDocumentModel(parent)
{
    setId(id);
    load();
}

SDocumentModel::~SDocumentModel()
{
}

int SDocumentModel::id()
{
    return i_id;
}

void SDocumentModel::setId(int id)
{
    i_id = id;
}

void SDocumentModel::load()
{
    QSqlQueryModel *docModel = new QSqlQueryModel();
    docModel->setQuery(QUERY_SEL_DOC(i_id), QSqlDatabase::database("connMain"));
    if(!docModel->rowCount())
        return;

    i_valuesMap.clear();
    m_type = docModel->record(0).value("type").toInt();
    m_state = docModel->record(0).value("state").toInt();
    m_realization = docModel->record(0).value("is_realization").toBool();
    m_paymentSystem = docModel->record(0).value("payment_system").toInt();
    m_company = docModel->record(0).value("company").toInt();
    m_store = docModel->record(0).value("store").toInt();
    m_user = docModel->record(0).value("user").toInt();
    m_amount = docModel->record(0).value("total").toInt();
    m_notes = docModel->record(0).value("notes").toInt();
    i_createdUtc = docModel->record(0).value("created").toDateTime();
    m_updated = docModel->record(0).value("updated_at").toDateTime();
    m_office = docModel->record(0).value("office").toInt();
    m_client = docModel->record(0).value("dealer").toInt();
    m_currencyRate = docModel->record(0).value("currency_rate").toFloat();
//    m_img1 = docModel->record(0).value("img1").toInt();
//    m_img2 = docModel->record(0).value("img2").toInt();
//    m_img3 = docModel->record(0).value("img3").toInt();
    m_reason = docModel->record(0).value("reason").toString();
    m_cashOrder = docModel->record(0).value("order_id").toInt();
    m_priceOption = docModel->record(0).value("price_option").toInt();
    m_returnPercent = docModel->record(0).value("return_percent").toInt();
    m_reserveDays = docModel->record(0).value("reserve_days").toInt();
    m_master = docModel->record(0).value("master_id").toInt();
    m_repair = docModel->record(0).value("repair_id").toInt();
    m_worksIncluded = docModel->record(0).value("works_included").toBool();
    m_bill = docModel->record(0).value("invoice").toInt();
    m_trackingNumber = docModel->record(0).value("track").toString();
    m_d_store = docModel->record(0).value("d_store").toInt();
    m_d_pay = docModel->record(0).value("d_pay").toInt();

    delete docModel;
}

void SDocumentModel::load(int id)
{
    setId(id);
    load();
}

void SDocumentModel::setType(int type)
{
    m_type = type;
    i_valuesMap.insert("type", type);
}

int SDocumentModel::state()
{
    return m_state;
}

void SDocumentModel::setState(int state)
{
    m_state = state;
    i_valuesMap.insert("state", state);
}

int SDocumentModel::client()
{
    return m_client;
}

void SDocumentModel::setClient(int id)
{
    if(id == 0)
    {
        unsetClient();
        return;
    }

    m_client = id;
    i_valuesMap.insert("dealer", id);
}

void SDocumentModel::unsetClient()
{
    i_valuesMap.insert("dealer", QVariant());
    i_logRecord->unsetClient();
}

int SDocumentModel::paymentSystem()
{
    return m_paymentSystem;
}

int SDocumentModel::paymentSystemIndex()
{
    return paymentSystemsModel->rowByDatabaseID(m_paymentSystem, "system_id");
}

void SDocumentModel::setPaymentSystem(int id)
{
    m_paymentSystem = id;
    i_valuesMap.insert("payment_system", id);
}

float SDocumentModel::amount()
{
    return m_amount;
}

QString SDocumentModel::amountLocal()
{
    return sysLocale.toString(m_amount, 'f', 2);
}

void SDocumentModel::setAmount(float amount)
{
    m_amount = amount;
    i_valuesMap.insert("total", amount);
}

float SDocumentModel::currencyRate()
{
    return m_currencyRate;
}

QString SDocumentModel::currencyRateLocal()
{
    return sysLocale.toString(m_currencyRate, 'f', 4);
}

void SDocumentModel::setCurrencyRate(float rate)
{
    m_currencyRate = rate;
    i_valuesMap.insert("currency_rate", rate);
}

int SDocumentModel::company()
{
    return m_company;
}

int SDocumentModel::companyIndex()
{
    return companiesModel->rowByDatabaseID(m_company, "id");
}

void SDocumentModel::setCompany(int company)
{
    m_company = company;
    i_valuesMap.insert("company", company);
}

int SDocumentModel::priceOption()
{
    return m_priceOption;
}

int SDocumentModel::priceOptionIndex()
{
    return priceColModel->rowByDatabaseID(m_priceOption, "id");
}

void SDocumentModel::setPriceOption(int option)
{
    m_priceOption = option;
    i_valuesMap.insert("price_option", option);
}

QString SDocumentModel::notes()
{
    return m_notes;
}

void SDocumentModel::setNotes(QString text)
{
    m_notes = text;
    i_valuesMap.insert("notes", text);
}

QString SDocumentModel::reason()
{
    return m_reason;
}

void SDocumentModel::setReason(QString text)
{
    m_reason = text;
    i_valuesMap.insert("reason", text);
}

int SDocumentModel::reserveDays()
{
    return m_reserveDays;
}

void SDocumentModel::setReserveDays(int days)
{
    m_reserveDays = days;
    i_valuesMap.insert("reserve_days", days);
}

QString SDocumentModel::trackingNumber()
{
    return m_trackingNumber;
}

void SDocumentModel::setTrackingNumber(const QString &track)
{
    m_trackingNumber = track;
    i_valuesMap.insert("track", track);
    if(track.isEmpty())
        appendLogText(tr("Номер ТТН для РН №%1 удалён").arg(i_id));
    else
        appendLogText(tr("РН №%1 присвоен номер ТТН: %2").arg(i_id).arg(track));
}

bool SDocumentModel::isValid()
{
    if (i_id)
        return 1;

    return 0;
}

bool SDocumentModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        i_valuesMap.insert("img1", QVariant()); // NULL
        i_valuesMap.insert("img2", QVariant());
        i_valuesMap.insert("img3", QVariant());
#ifdef  QT_DEBUG
        Q_ASSERT_X(userDbData->value("company").toInt(), objectName().toLocal8Bit(), QString("company not set").toLocal8Bit());
#endif
        i_valuesMap.insert("company", userDbData->value("company"));
        i_valuesMap.insert("office", userDbData->value("current_office"));
        i_valuesMap.insert("user", userDbData->value("id"));
        i_valuesMap.insert("created", QDateTime::currentDateTime());

        insert();
    }

    i_logRecord->setDocumentId(i_id);
    commitLogs();

    if(!i_nDBErr)
        throw 1;

    return i_nDBErr;
}
