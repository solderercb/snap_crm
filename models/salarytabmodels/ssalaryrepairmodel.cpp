#include "ssalaryrepairmodel.h"

SSalaryRepairModel::SSalaryRepairModel(QObject *parent) : SSingleRowModel(parent)
{
    mapFields();

    i_tableName = "salary_repairs";
    i_obligatoryFields << "user";
    setPrimaryKeyIndex(0);
}

