#include "scartridgerepairmodel.h"
#include "models/srepairmodel.h"

SCartridgeRepairModel::SCartridgeRepairModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "c_workshop";
    i_obligatoryFields << "card_id";
    i_idColumnName = "id";
}

SCartridgeRepairModel::~SCartridgeRepairModel()
{
}

int SCartridgeRepairModel::id()
{
    return i_id;
}

void SCartridgeRepairModel::load(const int id)
{
    i_id = id;

    if(!i_id)
    {
        loadError(Global::ThrowType::ConditionsError);
        return;
    }

    QSqlQuery record(QSqlDatabase::database("connMain"));
    record.exec(QUERY_SEL_CARTRIDGE_REPAIR_DATA(i_id));
    if(!record.first())
    {
        loadError(Global::ThrowType::ResultError);
        return;
    }

    m_refill = record.value("refill").toBool();
    m_chip = record.value("chip").toBool();
    m_drum = record.value("opc_drum").toBool();
    m_Blade = record.value("c_blade").toBool();
    m_cardId = record.value("card_id").toInt();
}

void SCartridgeRepairModel::loadError(const int type)
{
    QString msg = tr("Не удалось загрузить SCartridgeRepairModel; id записи в таблице `c_workshop`: %1, id ремонта: %2").arg(i_id).arg(m_repairId);
    Global::throwError(type, msg);
}

bool SCartridgeRepairModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        insert();
    }

    return 1;
}

bool SCartridgeRepairModel::refill()
{
    return m_refill;
}

void SCartridgeRepairModel::setRefill(const bool refill)
{
    i_valuesMap.insert("refill", refill);
}

bool SCartridgeRepairModel::chip()
{
    return m_chip;
}

void SCartridgeRepairModel::setChip(const bool chip)
{
    i_valuesMap.insert("chip", chip);
}

bool SCartridgeRepairModel::drum()
{
    return m_drum;
}

void SCartridgeRepairModel::setDrum(const bool opc_drum)
{
    i_valuesMap.insert("opc_drum", opc_drum);
}

bool SCartridgeRepairModel::Blade()
{
    return m_Blade;
}

void SCartridgeRepairModel::setBlade(const bool c_blade)
{
    i_valuesMap.insert("c_blade", c_blade);
}

int SCartridgeRepairModel::cardId()
{
    return m_cardId;
}

void SCartridgeRepairModel::setCardId(const int cardId)
{
    i_valuesMap.insert("card_id", cardId);
}

int SCartridgeRepairModel::repairId() const
{
    return m_repairId;
}

void SCartridgeRepairModel::setRepairId(int id)
{
    m_repairId = id;
}

