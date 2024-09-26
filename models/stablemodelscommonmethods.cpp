#include "global.h"
#include "stablemodelscommonmethods.h"

STableModelsCommonMethods::STableModelsCommonMethods(QObject *parent)
{
    Q_UNUSED(parent);
}

QDateTime STableModelsCommonMethods::timestampUtc(const QModelIndex &index) const
{
    QDateTime date = unformattedData(index, Qt::DisplayRole).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date;
}

QDateTime STableModelsCommonMethods::timestampLocal(const QModelIndex &index) const
{
    QDateTime date = unformattedData(index, Qt::DisplayRole).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date.toLocalTime();
}

double STableModelsCommonMethods::total(int column, int sign)
{
    double totalPositive = 0, totalNegative = 0;
    double current;
    for(int i = 0; i < this->rowCount_(); i++)
    {
        current = unformattedData(this->index_(i, column), Qt::DisplayRole).toDouble();
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

QString STableModelsCommonMethods::dataLocalizedFromDouble(const double value, const int decimals)
{
    return sysLocale.toString(value, 'f', decimals);
}

QString STableModelsCommonMethods::dataLocalizedFromDouble(const double value) const
{
    return dataLocalizedFromDouble(value, comSettings->classicKassa?2:0);
}

QString STableModelsCommonMethods::dataLocalizedFromDouble(const QModelIndex &item) const
{
    return dataLocalizedFromDouble(unformattedData(item, Qt::DisplayRole).toDouble(), comSettings->classicKassa?2:0);
}

QString STableModelsCommonMethods::companyFromId(const QModelIndex &item) const
{
    return companiesModel->getDisplayRole(unformattedData(item, Qt::DisplayRole).toInt());
}

QString STableModelsCommonMethods::userFromId(const QModelIndex &item) const
{
    return usersModel->getDisplayRole(unformattedData(item, Qt::DisplayRole).toInt());
}

QString STableModelsCommonMethods::warrantyFromId(const QModelIndex &item) const
{
    return warrantyTermsModel->getDisplayRole(unformattedData(item, Qt::DisplayRole).toInt(), 1);
}

QModelIndex STableModelsCommonMethods::index_(const int row, const int column) const
{
    if(m_sqlQueryModel)
        return m_sqlQueryModel->index(row, column);

    if(m_sqlFetchingModel)
        return m_sqlFetchingModel->index(row, column);

    return QModelIndex();
}

QVariant STableModelsCommonMethods::data(const int row, const int column, int role) const
{
    if(m_sqlQueryModel)
        return m_sqlQueryModel->data(this->index_(row, column), role);

    if(m_sqlFetchingModel)
        return m_sqlFetchingModel->data(this->index_(row, column), role);

    return QVariant();
}

int STableModelsCommonMethods::rowCount_()
{
    if(m_sqlQueryModel)
        return m_sqlQueryModel->rowCount();

    if(m_sqlFetchingModel)
        return m_sqlFetchingModel->rowCount();

    return 0;
}

const QMetaObject *STableModelsCommonMethods::metaObject()
{
    if(m_sqlQueryModel)
        return m_sqlQueryModel->metaObject();

    if(m_sqlFetchingModel)
        return m_sqlFetchingModel->metaObject();

    return 0;
}

QVariant STableModelsCommonMethods::data(const QModelIndex &index, int role) const
{
    return this->data(index.row(), index.column(), role);
}

QVariant STableModelsCommonMethods::unformattedData(const QModelIndex &item, int role) const
{
    return this->data(item, role | 0x0100);
}

QVariant STableModelsCommonMethods::unformattedData(const int row, const int column, int role) const
{
    return unformattedData(this->index_(row, column), role);
}

void STableModelsCommonMethods::setColumnWidth(const int column, const int width)
{
    i_columnWidths[column] = width;
}

int STableModelsCommonMethods::columnIndex(const QString &name)
{
    int enumIndex = this->metaObject()->indexOfEnumerator("Columns");
    if(enumIndex == -1)
    {
        qDebug().nospace() << "Failed to access to enum Columns in " << __FILE__ << ", line: " << __LINE__ << ": probably enumerator not declared with Q_ENUM()";
        return -1;
    }

    return this->metaObject()->enumerator(enumIndex).keyToValue(name.toLocal8Bit());
}

QString STableModelsCommonMethods::columnName(const int &index)
{
    int enumIndex = this->metaObject()->indexOfEnumerator("Columns");
    if(enumIndex == -1)
    {
        qDebug().nospace() << "Failed to access to enum Columns in " << __FILE__ << ", line: " << __LINE__ << ": probably enumerator not declared with Q_ENUM()";
        return "enumNA";
    }

    return this->metaObject()->enumerator(enumIndex).key(index);
}
