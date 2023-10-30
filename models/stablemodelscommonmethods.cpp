#include "global.h"
#include "stablemodelscommonmethods.h"

STableModelsCommonMethods::STableModelsCommonMethods(QObject *parent)
{
    Q_UNUSED(parent);
}

int STableModelsCommonMethods::field(const QString &field)
{
    return i_fields[field];
}

QDateTime STableModelsCommonMethods::timestampUtc(const QModelIndex &index) const
{
    QDateTime date = derivedModel->data(index, Qt::DisplayRole | 0x0100).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date;
}

QDateTime STableModelsCommonMethods::timestampLocal(const QModelIndex &index) const
{
    QDateTime date = derivedModel->data(index, Qt::DisplayRole | 0x0100).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date.toLocalTime();
}

double STableModelsCommonMethods::total(int column, int sign)
{
    double totalPositive = 0, totalNegative = 0;
    double current;
    for(int i = 0; i < derivedModel->rowCount(); i++)
    {
        current = derivedModel->data(derivedModel->index(i, column), Qt::DisplayRole | 0x0100).toDouble();
        if(sign != 0)
        {
            totalPositive += (current>0)?current:0;
            totalNegative += (current<0)?current:0;
        }
        else
            totalPositive += current;
    }

    if(sign >= 0)
        return totalPositive;
    else
        return totalNegative;
}

QString STableModelsCommonMethods::dataLocalizedFromDouble(const QModelIndex &item) const
{
    return sysLocale.toString(derivedModel->data(item, Qt::DisplayRole | 0x0100).toDouble(), 'f', comSettings->classicKassa?2:0);
}

QString STableModelsCommonMethods::userFromId(const QModelIndex &item) const
{
    return usersModel->getDisplayRole(derivedModel->data(item, Qt::DisplayRole | 0x0100).toInt());
}

QString STableModelsCommonMethods::warrantyFromId(const QModelIndex &item) const
{
    return warrantyTermsModel->getDisplayRole(derivedModel->data(item, Qt::DisplayRole | 0x0100).toInt(), 1);
}

QVariant STableModelsCommonMethods::data(const int row, const int column, int role) const
{
    return derivedModel->data(derivedModel->index(row, column), role);
}

QVariant STableModelsCommonMethods::unformattedData(const QModelIndex &item, int role) const
{
    return derivedModel->data(item, role | 0x0100);
}

QVariant STableModelsCommonMethods::unformattedData(const int row, const int column, int role) const
{
    return derivedModel->data(derivedModel->index(row, column), role | 0x0100);
}

void STableModelsCommonMethods::setColumnWidth(const int column, const int width)
{
    i_columnWidths[column] = width;
}

void STableModelsCommonMethods::cashFieldsNames()
{
    i_fields.clear();

    if(derivedModel->rowCount() == 0)
        return;

    for(int i = 0; i < derivedModel->record(0).count(); i++)
    {
        i_fields.insert(derivedModel->record(0).fieldName(i), i);
    }
}

