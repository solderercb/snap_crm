#include "sinvoicemodel.h"

SInvoiceModel::SInvoiceModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "invoice";
    i_obligatoryFields << "num" << "created" << "user" << "seller" << "customer" << "office";
}

SInvoiceModel::~SInvoiceModel()
{

}

void SInvoiceModel::load(int id)
{
    QSqlQueryModel *invoiceModel = new QSqlQueryModel(this);

    invoiceModel->setQuery(QUERY_SEL_INVOICE(id), QSqlDatabase::database("connMain"));
    if(invoiceModel->rowCount())
    {
        setId(id);
        i_valuesMap.clear();
        i_logRecord->setClient(id);
        m_num = invoiceModel->record(0).value("num").toString();
        m_created = invoiceModel->record(0).value("created").toDateTime();
        m_user = invoiceModel->record(0).value("user").toInt();
        m_seller = invoiceModel->record(0).value("seller").toInt();
        m_covenantorId = invoiceModel->record(0).value("covenantor").toInt();
        m_covenantorName = invoiceModel->record(0).value("covenantor_name").toString();
        m_clientId = invoiceModel->record(0).value("client_id").toInt();
        m_paid = invoiceModel->record(0).value("paid").toDateTime();
        m_tax = invoiceModel->record(0).value("tax").toDouble();
        m_summ = invoiceModel->record(0).value("summ").toDouble();
        m_notes = invoiceModel->record(0).value("notes").toString();
        m_total = invoiceModel->record(0).value("total").toDouble();
        m_state = invoiceModel->record(0).value("state").toInt();
        m_office = invoiceModel->record(0).value("office").toInt();
        m_type = invoiceModel->record(0).value("type").toInt();
    }

    delete invoiceModel;
}

int SInvoiceModel::id()
{
    return i_id;
}

void SInvoiceModel::setId(const int id)
{
    i_id = id;
}

QString SInvoiceModel::num()
{
    return m_num;
}

void SInvoiceModel::setNum(const QString num)
{
    i_valuesMap.insert("num", num);
}

QDateTime SInvoiceModel::created()
{
    return m_created;
}

void SInvoiceModel::setCreated(const QDateTime created)
{
    i_valuesMap.insert("created", created);
}

int SInvoiceModel::user()
{
    return m_user;
}

void SInvoiceModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

int SInvoiceModel::seller()
{
    return m_seller;
}

void SInvoiceModel::setSeller(const int seller)
{
    i_valuesMap.insert("seller", seller);
}

int SInvoiceModel::covenantorId()
{
    return m_covenantorId;
}

QString SInvoiceModel::covenantorName()
{
    return m_covenantorName;
}

void SInvoiceModel::setCovenantor(const int id)
{
    i_valuesMap.insert("customer", id);
}

int SInvoiceModel::clientId()
{
    return m_clientId;
}

QDateTime SInvoiceModel::paid()
{
    return m_paid;
}

void SInvoiceModel::setPaid(const QDateTime paid)
{
    i_valuesMap.insert("paid", paid);
}

double SInvoiceModel::tax()
{
    return m_tax;
}

void SInvoiceModel::setTax(const double tax)
{
    i_valuesMap.insert("tax", tax);
}

double SInvoiceModel::summ()
{
    return m_summ;
}

void SInvoiceModel::setSumm(const double summ)
{
    i_valuesMap.insert("summ", summ);
}

QString SInvoiceModel::notes()
{
    return m_notes;
}

void SInvoiceModel::setNotes(const QString notes)
{
    i_valuesMap.insert("notes", notes);
}

double SInvoiceModel::total()
{
    return m_total;
}

void SInvoiceModel::setTotal(const double total)
{
    i_valuesMap.insert("total", total);
}

int SInvoiceModel::state()
{
    return m_state;
}

void SInvoiceModel::setState(const int state)
{
    i_valuesMap.insert("state", state);
}

int SInvoiceModel::office()
{
    return m_office;
}

void SInvoiceModel::setOffice(const int office)
{
    i_valuesMap.insert("office", office);
}

int SInvoiceModel::type()
{
    return m_type;
}

void SInvoiceModel::setType(const int type)
{
    i_valuesMap.insert("type", type);
}

bool SInvoiceModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        setCreated(QDateTime::currentDateTime());
        setUser(userDbData->value("id").toInt());
        setOffice(userDbData->value("current_office").toInt());

        insert();
    }

    return i_nErr;
}

