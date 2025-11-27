#include "sdevmdlmodel.h"
#include <SLogRecordModel>

SDevMdlModel::SDevMdlModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "maker";
    i_tableName = "device_models";
    setPrimaryKeyIndex(0);
    i_logRecord->set_group(SLogRecordModel::RecordTypes::ACP);
//    i_logRecord->set_type(1);
}

SDevMdlModel::~SDevMdlModel()
{

}

// TODO: подумать над необходимостью записи в журнал, например, "Добавлена модель %1 в группу %2/%3"
