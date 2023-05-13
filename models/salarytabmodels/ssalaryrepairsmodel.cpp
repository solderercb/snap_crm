#include "ssalaryrepairsmodel.h"

SSalaryRepairsModel::SSalaryRepairsModel(QObject *parent) : SDatabaseRecord(parent)
{
    i_tableName = "salary_repairs";
    i_obligatoryFields << "user";
}

int SSalaryRepairsModel::id()
{
    return i_id;
}

int SSalaryRepairsModel::repair()
{
    return m_repair;
}

void SSalaryRepairsModel::setRepair(const int repair)
{
    i_valuesMap.insert("repair", repair);
}

int SSalaryRepairsModel::cartridge()
{
    return m_cartridge;
}

void SSalaryRepairsModel::setCartridge(const int cartridge)
{
    i_valuesMap.insert("cartridge", cartridge);
}

int SSalaryRepairsModel::user()
{
    return m_user;
}

void SSalaryRepairsModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

double SSalaryRepairsModel::summ()
{
    return m_summ;
}

void SSalaryRepairsModel::setSumm(const double summ)
{
    i_valuesMap.insert("summ", summ);
}

bool SSalaryRepairsModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        insert();
    }

    if(!i_nErr)
        throw 1;

    return i_nErr;
}

