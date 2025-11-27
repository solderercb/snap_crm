#include "sinvoicemodel.h"
#include <SUserSettings>
#include <ProjectGlobals>
#include <ProjectQueries>

SInvoiceModel::SInvoiceModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "invoice";
    i_obligatoryFields << "num" << "created" << "user" << "seller" << "customer" << "office";
    setPrimaryKeyIndex(0);
}

SInvoiceModel::~SInvoiceModel()
{

}

QString SInvoiceModel::constructSelectQuery()
{
    // Составленный вручную запрос SELECT, содержащий доп. поля из другой таблицы.
    // Не повлияет на работу при соблюдении условий:
    //  - запрос должен содержать все поля основной таблицы, включая id;
    //  - порядок должен совпадать с TABLE_FIELDS
    //  - к полям основной таблицы не должны применяться алиасы (например, `name` AS 'new_name')
    //  - доп. поля должны быть перечислены после основных;
    return QUERY_SEL_INVOICE(primaryKeyData().toInt());
}

void SInvoiceModel::load(int id)
{
    setPrimaryKey(id);
    SSingleRowModel::load();
}

QString SInvoiceModel::covenantorName()
{
    if(!isModelLoaded())
        return QString();

    return m_record->value("customer_name").toString();
}

int SInvoiceModel::clientId()
{
    if(!isModelLoaded())
        return 0;

    return m_record->value("client_id").toInt();
}

bool SInvoiceModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        set_created(QDateTime::currentDateTime());
        set_user(userDbData->id());
        initMandatoryField(C_office, userDbData->currentOffice());
    }

    return SSingleRowModel::commit();
}

