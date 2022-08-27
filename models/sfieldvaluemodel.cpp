#include "sfieldvaluemodel.h"

SFieldValueModel::SFieldValueModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "field_id" << "value";
    tableName = "field_values";
}

SFieldValueModel::SFieldValueModel(const int id, QObject *parent) : SFieldValueModel()
{
    load(id);
}

SFieldValueModel::~SFieldValueModel()
{

}

void SFieldValueModel::load(const int id)
{
    SSqlQueryModel *fieldModel = new SSqlQueryModel(this);
    fieldModel->setQuery(QUERY_SEL_ADD_FIELD(i_id));
    if(fieldModel->lastError().isValid())
        return;
    m_isValid = 1;
    load(fieldModel->record(0));
}

void SFieldValueModel::load(QSqlRecord record)
{
    i_valuesMap.clear();

    i_id = record.value("id").toInt();
    m_name = record.value("name").toString();
    m_fieldId = record.value("field_id").toInt();
    m_repairId = record.value("repair_id").toInt();
    m_itemId = record.value("item_id").toInt();
    m_value = record.value("value").toString();
}

int SFieldValueModel::id()
{
    return m_id;
}

void SFieldValueModel::setId(const int id)
{
    i_valuesMap.insert("id", id);
}

int SFieldValueModel::fieldId()
{
    return m_fieldId;
}

void SFieldValueModel::setFieldId(const int field_id)
{
    i_valuesMap.insert("field_id", field_id);
}

int SFieldValueModel::repairId()
{
    return m_repairId;
}

void SFieldValueModel::setRepairId(const int repair_id)
{
    i_valuesMap.insert("repair_id", repair_id);
    i_logRecord->setRepairId(repair_id);
}

int SFieldValueModel::itemId()
{
    return m_itemId;
}

void SFieldValueModel::setItemId(const int item_id)
{
    i_valuesMap.insert("item_id", item_id);
    i_logRecord->setItemId(item_id);
}

QString SFieldValueModel::value()
{
    return m_value;
}

void SFieldValueModel::setValue(const QString& value)
{
    if(i_id)
        appendLogText(tr("Значение поля %1 изменено с %2 на %2").arg(m_name,m_value,value));

    i_valuesMap.insert("value", value);
}

bool SFieldValueModel::commit()
{
    if(i_id)
        update();
    else
        insert();
    commitLogs();
    return i_nDBErr;
}

bool SFieldValueModel::delDBRecord()
{
    i_logRecord->setText(tr("Параметр %1 (%2) удалён").arg(m_name, m_value));
    i_logRecord->commit();
    return SComRecord::del();
}

bool SFieldValueModel::isValid()
{
    return m_isValid;
}

