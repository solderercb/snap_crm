#include "scartridgerepairmodel.h"
#include "models/srepairmodel.h"

SCartridgeRepairModel::SCartridgeRepairModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "c_workshop";
    i_obligatoryFields << "card_id";
    i_idColumnName = "id";
}

SCartridgeRepairModel::SCartridgeRepairModel(const int id, QObject *parent) :
    SCartridgeRepairModel(parent)
{
    load(id);
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

    if(!id)
    {
        loadError();
        return;
    }

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QUERY_SEL_CARTRIDGE_REPAIR_DATA(i_id));
    if(!record->first())
    {
        loadError();
        return;
    }

    m_refill = record->value("refill").toBool();
    m_chip = record->value("chip").toBool();
    m_drum = record->value("opc_drum").toBool();
    m_Blade = record->value("c_blade").toBool();
    m_cardId = record->value("card_id").toInt();

    delete record;
}

// TODO: аналогичный метод есть в классе SCartridgeCardModel; нужно их обобщить
void SCartridgeRepairModel::loadError()
{
    SRepairModel* repair = dynamic_cast<SRepairModel*>(parent());
    if(repair)
    {
        appLog->appendRecord(tr("Не удалось инициализировать модель SCartridgeRepairModel ремонта №%1").arg(repair->id()));
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ошибка", "Не удалось загрузить информацию о картридже. Обратитесь к администратору", QColor("#FFC7AD"), QColor("#FFA477"));
    }
}

bool SCartridgeRepairModel::commit()
{
    if(i_id)
    {
        if(!update())
            throw Global::ThrowType::QueryError;
    }
    else
    {
        if(!insert())
            throw Global::ThrowType::QueryError;
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

