#include "srepairstatuslog.h"
#include <SUserSettings>
#include <ProjectGlobals>
#include <SSqlQueryModel>

SRepairStatusLog::SRepairStatusLog(QObject *parent):
    SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "created_at" << "repair_id" << "status_id" << "user_id";
    i_tableName = "repair_status_logs";

    i_primaryKeyIndex = 0;
    connect(this, &SSingleRowModel::beginDataChange, this, &SRepairStatusLog::setDataRework);
}

void SRepairStatusLog::set_managerIndex(const int index)
{
    if(index == -1)
        set_manager(0);
    else
        set_manager(managersModel->databaseIDByRow(index));
}

void SRepairStatusLog::set_engineerIndex(const int index)
{
    if(index == -1)
        set_engineer(0);
    else
        set_engineer(engineersModel->databaseIDByRow(index));
}

bool SRepairStatusLog::commit()
{
    if(!isDirty())
        return 1;

    initMandatoryField(C_user, userDbData->id());
    initMandatoryField(C_created, QDateTime::currentDateTime());
    if(!insert())
        return 0;
    setFieldsExecuted();

    return 1;
}

void SRepairStatusLog::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_manager: if(!data.toInt()) data = QVariant(); break;
        case C_engineer: if(!data.toInt()) data = QVariant(); break;
        default: ;
    }
}

/* Переопределённый метод
 * Требуется пометка только поля status.
 * В тестах это позволит проверить последнюю запись на предмет правильно отработавших алгоритмов коммита
*/
void SRepairStatusLog::setAllState(ModifiedField::State state)
{
    CacheMap::iterator i = cache.find(C_status);
    if(i != cache.end())
    {
        setState(*i, state);
    }
}

/* Переопределённый метод
 * Проверяется только поле status.
*/
bool SRepairStatusLog::isDirty()
{
    return isFieldDirty(C_status);
}
