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
    void clear();
    QString userFromId(const QModelIndex &item) const;
    QString warrantyFromId(const QModelIndex &item) const;
private:
protected:
    QMap<QString, int> i_fields;
    QSqlQueryModel *derivedModel;
public slots:
    void cashFieldsNames();
};

#endif // STABLEMODELSCOMMONMETHODS_H
