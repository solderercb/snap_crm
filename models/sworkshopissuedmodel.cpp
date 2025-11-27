#include "sworkshopissuedmodel.h"
#include <SUserSettings>
#include <ProjectGlobals>

SWorkshopIssuedModel::SWorkshopIssuedModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "repair_id" << "employee_id" << "created_at";
    i_tableName = "workshop_issued";
    i_primaryKeyIndex = 0;
}

bool SWorkshopIssuedModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_employee, userDbData->id());
        initMandatoryField(C_created, QDateTime::currentDateTime());
    }

    SSingleRowJModel::commit();

    return 1;
}

