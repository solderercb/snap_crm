#include "sdevmdlmodel.h"

SDevMdlModel::SDevMdlModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "maker";
    i_tableName = "device_models";
    i_idColumnName = "id";
    i_logRecord->setGroup(1);
//    i_logRecord->setType(1);
}

SDevMdlModel::~SDevMdlModel()
{

}

bool SDevMdlModel::commit()
{
    if(i_id)
        update();
    else
    {
        insert();
    }
    commitLogs();

    if(!i_nErr)
        throw 1;

    return i_nErr;
}

int SDevMdlModel::id()
{
    return i_id;
}

void SDevMdlModel::setId(const int id)
{
    i_valuesMap.insert("id", id);
}

QString SDevMdlModel::name()
{
    return m_name;
}

void SDevMdlModel::setName(const QString name)
{
    i_valuesMap.insert("name", name);
    // TODO: подумать над необходимостью записи в журнал, например, "Добавлена модель %1 в группу %2/%3"
}

int SDevMdlModel::position()
{
    return m_position;
}

void SDevMdlModel::setPosition(const int id)
{
    i_valuesMap.insert("position", id);
}

int SDevMdlModel::maker()
{
    return m_maker;
}

void SDevMdlModel::setMaker(const int id)
{
    i_valuesMap.insert("maker", id);
}

int SDevMdlModel::device()
{
    return m_device;
}

void SDevMdlModel::setDevice(const int id)
{
    i_valuesMap.insert("device", id);
}

