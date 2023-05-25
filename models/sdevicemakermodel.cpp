#include "sdevicemakermodel.h"

SDeviceMakerModel::SDeviceMakerModel(QObject *parent) : SComRecord(parent)
{
//    i_obligatoryFields << "name" << "notes";
    i_tableName = "device_makers";
    i_idColumnName = "id";
}

SDeviceMakerModel::~SDeviceMakerModel()
{

}

int SDeviceMakerModel::id()
{
    return m_id;
}

void SDeviceMakerModel::setId(int id)
{
    i_valuesMap.insert("id", id);
}

QString SDeviceMakerModel::name()
{
    return m_name;
}

void SDeviceMakerModel::setName(QString name)
{
    i_valuesMap.insert("name", name);
}

