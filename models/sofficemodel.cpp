#include "sofficemodel.h"
#include "global.h"

SOfficeModel* SOfficeModel::p_current = nullptr;


SOfficeModel::SOfficeModel(QObject *parent) : SComRecord(parent)
{

}

SOfficeModel::~SOfficeModel()
{
    p_current = nullptr;
}

SOfficeModel *SOfficeModel::current(QObject *parent)
{
    if( !p_current )
        p_current = new SOfficeModel(parent);

    if(p_current)
        if( userDbData->currentOffice > 0 && p_current->id() != userDbData->currentOffice )
            p_current->load(userDbData->currentOffice);

    return p_current;
}

void SOfficeModel::load(const int &id)
{
    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("SELECT  `state`, `name`, `address`, `phone`, `phone2`, `logo`, `administrator`, `created`, `default_company`, `card_payment`, `use_boxes`, `paint_repairs`, `warranty_sn` FROM `offices` WHERE `id` = %1 AND `state` = 1;").arg(id));

    if(record->first())
    {
        i_id = id;
        i_valuesMap.clear();
        i_logRecord->setClient(id);
        m_state = record->value("state").toInt();
        m_name = record->value("name").toString();
        m_address = record->value("address").toString();
        m_phone = record->value("phone").toString();
        m_phone2 = record->value("phone2").toString();
        m_logo = record->value("logo").toByteArray();
        m_admin = record->value("administrator").toInt();
        i_createdUtc = record->value("created").toDateTime();
        m_defaultCompany = record->value("default_company").toInt();
        m_cardPayment = record->value("card_payment").toBool();
        m_useBoxes = record->value("use_boxes").toBool();
        m_paintRepairs = record->value("paint_repairs").toBool();
        m_warrantySN = record->value("warranty_sn").toBool();
    }

    delete record;
}

const int& SOfficeModel::id()
{
    return i_id;
}

const int& SOfficeModel::state()
{
    return m_state;
}

void SOfficeModel::setState(const int &state)
{
    i_valuesMap.insert("state", state);
}

const QString& SOfficeModel::name()
{
    return m_name;
}

void SOfficeModel::setName(const QString &name)
{
    i_valuesMap.insert("name", name);
}

const QString& SOfficeModel::address()
{
    return m_address;
}

void SOfficeModel::setAddress(const QString &address)
{
    i_valuesMap.insert("address", address);
}

const QString& SOfficeModel::phone()
{
    return m_phone;
}

void SOfficeModel::setPhone(const QString &phone)
{
    i_valuesMap.insert("phone", phone);
}

const QString& SOfficeModel::phone2()
{
    return m_phone2;
}

void SOfficeModel::setPhone2(const QString &phone2)
{
    i_valuesMap.insert("phone2", phone2);
}

const QByteArray& SOfficeModel::logo()
{
    return m_logo;
}

void SOfficeModel::setLogo(const QByteArray &logo)
{
    i_valuesMap.insert("logo", logo);
}

const int& SOfficeModel::admin()
{
    return m_admin;
}

void SOfficeModel::setAdmin(const int &id)
{
    i_valuesMap.insert("administrator", id);
}

int SOfficeModel::defaultCompany()
{
    return m_defaultCompany;
}

int SOfficeModel::defaultCompanyIndex()
{
    return companiesModel->rowByDatabaseID(m_defaultCompany);
}

void SOfficeModel::setDefaultCompany(const int &id)
{
    i_valuesMap.insert("default_company", id);
}

const bool& SOfficeModel::cardPayment()
{
    return m_cardPayment;
}

void SOfficeModel::setCardPayment(const bool &state)
{
    i_valuesMap.insert("card_payment", state);
}

const bool& SOfficeModel::useBoxes()
{
    return m_useBoxes;
}

void SOfficeModel::setUseBoxes(const bool &state)
{
    i_valuesMap.insert("use_boxes", state);
}

const bool& SOfficeModel::paintRepairs()
{
    return m_paintRepairs;
}

void SOfficeModel::setPaintRepairs(const bool &state)
{
    i_valuesMap.insert("paint_repairs", state);
}

const bool& SOfficeModel::warrantySN()
{
    return m_warrantySN;
}

void SOfficeModel::setWarrantySN(const bool &state)
{
    i_valuesMap.insert("warranty_sn", state);
}
