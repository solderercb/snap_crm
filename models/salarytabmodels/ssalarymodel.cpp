#include "ssalarymodel.h"
#include <ProjectGlobals>

SSalaryModel::SSalaryModel(QObject *parent) : SSingleRowModel(parent)
{
    mapFields();

    i_tableName = "salary";
    i_obligatoryFields << "user_id" << "summ" << "balance" << "payment_date" << "notes" << "period_from" << "period_to" << "from_user" << "type";
    setPrimaryKeyIndex(0);
}

void SSalaryModel::load(const int id)
{
    if(!id)
        return;

    setPrimaryKey(id);
    SSingleRowModel::load();
}
