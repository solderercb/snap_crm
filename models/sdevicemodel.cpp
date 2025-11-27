#include "sdevicemodel.h"

SDeviceModel::SDeviceModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "fault_list" << "look_list" << "complect_list" << "company_list";
    i_tableName = "devices";
    setPrimaryKeyIndex(0);
}

SDeviceModel::~SDeviceModel()
{

}
