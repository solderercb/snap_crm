/*  Общая модель данных таблиц.
*/
#ifndef STABLEMODELSCOMMONMETHODS_H
#define STABLEMODELSCOMMONMETHODS_H

#include <QObject>
#include <QSqlQueryModel>
#include <QDebug>

class STableModelsCommonMethods
{
public:
    explicit STableModelsCommonMethods(QObject *parent = nullptr);
    int field(const QString &field);
    QDateTime timestampUtc(const QModelIndex &index) const;
    QDateTime timestampLocal(const QModelIndex &index) const;
    double total(int column, int sign = 0);
    QString dataLocalizedFromDouble(const QModelIndex &item) const;
    QString companyFromId(const QModelIndex &item) const;
    QString userFromId(const QModelIndex &item) const;
    QString warrantyFromId(const QModelIndex &item) const;
    QVariant data(const int row, const int column, int role = Qt::DisplayRole) const;
    QVariant unformattedData(const QModelIndex &item, int role = Qt::DisplayRole) const;
    QVariant unformattedData(const int row, const int column, int role = Qt::DisplayRole) const;
    void setColumnWidth(const int column, const int width);
private:
protected:
    QMap<QString, int> i_fields;
    QMap<int, int> i_columnWidths;  // ширина столбца в символах
    QSqlQueryModel *derivedModel;
public slots:
    void cashFieldsNames();
};

#endif // STABLEMODELSCOMMONMETHODS_H
