#include "sdevicemakermodel.h"

SDeviceMakerModel::SDeviceMakerModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();
//    i_obligatoryFields << "name" << "notes";
    i_tableName = "device_makers";
    setPrimaryKeyIndex(0);
}

SDeviceMakerModel::~SDeviceMakerModel()
{

}

