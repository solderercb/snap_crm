#include "srepairstatuslog.h"

SRepairStatusLog::SRepairStatusLog(const int repair, QObject *parent):
    SComRecord(parent),
    m_repair(repair)
{
    i_obligatoryFields << "created_at" << "repair_id" << "status_id" << "user_id";
    i_tableName = "repair_status_logs";
}

int SRepairStatusLog::id()
{
    return i_id;
}

void SRepairStatusLog::setCreated(const QDateTime created_at)
{
    i_valuesMap.insert("created_at", created_at);
}

int SRepairStatusLog::repair()
{
    return m_repair;
}

void SRepairStatusLog::setRepair(const int repair_id)
{
    i_valuesMap.insert("repair_id", repair_id);
}

int SRepairStatusLog::status()
{
    return m_status;
}

void SRepairStatusLog::setStatus(const int status_id)
{
    m_changed = 1;
    i_id = 0;
    i_valuesMap.insert("status_id", status_id);
}

int SRepairStatusLog::user()
{
    return m_user;
}

void SRepairStatusLog::setUser(const int user_id)
{
    i_valuesMap.insert("user_id", user_id);
}

int SRepairStatusLog::manager()
{
    return m_manager;
}

void SRepairStatusLog::setManager(const QVariant value)
{
    i_valuesMap.insert("manager_id", value);
}

void SRepairStatusLog::setManager(const int id)
{
    if(!id)
        setManager(QVariant());
    else
        setManager(QVariant(id));
}

void SRepairStatusLog::setManagerIndex(const int index)
{
    if(index == -1)
        setManager(QVariant());
    else
    setManager(managersModel->databaseIDByRow(index));
}

int SRepairStatusLog::engineer()
{
    return m_master;
}

void SRepairStatusLog::setEngineer(const QVariant value)
{
    i_valuesMap.insert("master_id", value);
}

void SRepairStatusLog::setEngineer(const int id)
{
    if(!id)
        setEngineer(QVariant());
    else
        setEngineer(QVariant(id));
}

void SRepairStatusLog::setEngineerIndex(const int index)
{
    if(index == -1)
        setEngineer(QVariant());
    else
        setEngineer(engineersModel->databaseIDByRow(index));
}

void SRepairStatusLog::load(const int id)
{
    if(!id)
        return;

    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_RPRT(id));
    if(!query->first())
        return;

    i_id = id;
    i_createdUtc = query->value("created_at").toDateTime();
    m_repair = query->value("repair_id").toInt();
    m_status = query->value("status_id").toInt();
    m_user = query->value("user_id").toInt();
    m_manager = query->value("manager_id").toInt();
    m_master = query->value("master_id").toInt();
}

bool SRepairStatusLog::commit()
{
    if(!m_changed)
        return 1;

    if(i_id)
    {
        update();
    }
    else
    {
        if(m_repair)
            setRepair(m_repair);
        setUser(userDbData->value("id").toInt());
        setCreated(QDateTime::currentDateTime());
        insert();
    }
    return i_nErr;
}

