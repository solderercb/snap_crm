/*  Общая модель данных таблиц.
*/
#ifndef STABLEMODELSCOMMONMETHODS_H
#define STABLEMODELSCOMMONMETHODS_H

#include <QObject>
#include <QSqlQueryModel>
#include <QDebug>
#include "models/ssqlfetchingmodel.h"

class STableModelsCommonMethods
{
public:
    explicit STableModelsCommonMethods(QObject *parent = nullptr);
    QDateTime timestampUtc(const QModelIndex &index) const;
    QDateTime timestampLocal(const QModelIndex &index) const;
    double total(int column, int sign = 0);
    static QString dataLocalizedFromDouble(const double value, const int decimals);
    QString dataLocalizedFromDouble(const double value) const;
    QString dataLocalizedFromDouble(const QModelIndex &item) const;
    QString companyFromId(const QModelIndex &item) const;
    QString userFromId(const QModelIndex &item) const;
    QString warrantyFromId(const QModelIndex &item) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant data(const int row, const int column, int role = Qt::DisplayRole) const;
    QVariant unformattedData(const QModelIndex &item, int role = Qt::DisplayRole) const;
    QVariant unformattedData(const int row, const int column, int role = Qt::DisplayRole) const;
    QVariant dataShort(const QModelIndex &index) const;
    void setColumnWidth(const int column, const int width);
    void setRecordValue(QSqlRecord &record, const int field, const QVariant &value);
private:
    QModelIndex index_(const int row, const int column) const;
    int rowCount_();
protected:
    QMap<int, int> i_columnWidths;  // ширина столбца в символах
    QSqlQueryModel *m_sqlQueryModel = nullptr;
    SSqlFetchingModel *m_sqlFetchingModel = nullptr;
    int columnIndex(const QString &name);
    QString columnName(const int &index);
    const QMetaObject *metaObject();
    virtual QModelIndex indexForShortData(const QModelIndex &index) const;
};

#endif // STABLEMODELSCOMMONMETHODS_H
