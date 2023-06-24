#include "sworkshopissuedmodel.h"

SWorkshopIssuedModel::SWorkshopIssuedModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "repair_id" << "employee_id" << "created_at";
    i_tableName = "workshop_issued";
    i_idColumnName = "id";
}

int SWorkshopIssuedModel::id()
{
    return i_id;
}

int SWorkshopIssuedModel::repair()
{
    return m_repair;
}

void SWorkshopIssuedModel::setRepair(const int id)
{
    i_valuesMap.insert("repair_id", id);
}

int SWorkshopIssuedModel::employee()
{
    return m_employeeId;
}

void SWorkshopIssuedModel::setEmployee(const int id)
{
    i_valuesMap.insert("employee_id", id);
}

void SWorkshopIssuedModel::load(const int id)
{
    if(!id)
        return;

    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_RPRT(id));
    if(!query->first())
        return;

    i_id = id;
    m_repair = query->value("repair_id").toInt();
    m_employeeId = query->value("employee_id").toInt();
    i_createdUtc = query->value("created_at").toDateTime();
}

bool SWorkshopIssuedModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        setEmployee(userDbData->id);
        i_valuesMap.insert("created_at", QDateTime::currentDateTime());
        insert();
    }
    return i_nErr;
}

