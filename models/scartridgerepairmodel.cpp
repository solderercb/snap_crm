#include "scartridgerepairmodel.h"
#include <ProjectGlobals>

SCartridgeRepairModel::SCartridgeRepairModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "c_workshop";
    i_obligatoryFields << "card_id";
    setPrimaryKeyIndex(0);
}

SCartridgeRepairModel::~SCartridgeRepairModel()
{
}

void SCartridgeRepairModel::load(const int id)
{
    if(!id)
    {
        loadError(Global::ThrowType::ConditionsError);
        return;
    }

    setPrimaryKey(id);
    SSingleRowJModel::load();
}

void SCartridgeRepairModel::loadError(const int type)
{
    QString msg = tr("Не удалось загрузить SCartridgeRepairModel; id записи в таблице `c_workshop`: %1, id ремонта: %2").arg(primaryKeyData().toInt()).arg(m_repairId);
    Global::throwError(type, msg);
}

int SCartridgeRepairModel::repairId() const
{
    return m_repairId;
}

void SCartridgeRepairModel::setRepairId(int id)
{
    m_repairId = id;
}

