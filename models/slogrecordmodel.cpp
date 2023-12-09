#include "slogrecordmodel.h"

SLogRecordModel::SLogRecordModel(QObject *parent) :
    SDatabaseRecord(parent)
{
    i_obligatoryFields << "user" << "created" << "values" << "values_after" << "office" << "notes";
    i_tableName = "logs";
}

bool SLogRecordModel::commit()
{
    if(!i_valuesMap.contains("user"))
        i_valuesMap.insert("user", userDbData->id);
    i_valuesMap.insert("values", QVariant());
    i_valuesMap.insert("values_after", QVariant());
    if(!i_valuesMap.contains("office"))
        i_valuesMap.insert("office", userDbData->currentOffice);
    i_valuesMap.insert("created", QDateTime::currentDateTime());

    insert(false);

    return i_nErr;
}

bool SLogRecordModel::commit(const QString &text)
{
    setText(text);
    return commit();
}

void SLogRecordModel::setText(const QString &text)
{
    i_valuesMap.insert("notes", text);
}

/*  Установка кода валюты
 *  (это задел на бивалютную или мультиваллютную систему)
 */
void SLogRecordModel::setCurrencyId(int id)
{
    i_valuesMap.insert("currency", id);
}

void SLogRecordModel::setRepairId(int id)
{
    i_valuesMap.insert("repair", id);
}

void SLogRecordModel::setDocumentId(int id)
{
    i_valuesMap.insert("document", id);
}

void SLogRecordModel::setItemId(int id)
{
    i_valuesMap.insert("item", id);
}

void SLogRecordModel::setCashOrderId(int id)
{
    i_valuesMap.insert("cash_order", id);
}

void SLogRecordModel::setPartRequestId(int id)
{
    i_valuesMap.insert("part_request", id);
}

void SLogRecordModel::setValueBefore(const QString &text)
{
    i_valuesMap.insert("values", text);
}

void SLogRecordModel::setValueAfter(const QString &text)
{
    i_valuesMap.insert("values_after", text);
}

void SLogRecordModel::setOffice(const int id)
{
    i_valuesMap.insert("office", id);
}

void SLogRecordModel::setUser(const int id)
{
    i_valuesMap.insert("user", id);
}

void SLogRecordModel::clear()
{
    i_valuesMap.clear();
}

void SLogRecordModel::setClient(int id)
{
    if(id == 0)
    {
        unsetClient();
        return;
    }

    i_valuesMap.insert("client", id);
}

void SLogRecordModel::unsetClient()
{
    i_valuesMap.insert("client", QVariant());
}

void SLogRecordModel::setType(int id)
{
    i_valuesMap.insert("type", id);
}

void SLogRecordModel::setGroup(int id)
{
    i_valuesMap.insert("group", id);
}
