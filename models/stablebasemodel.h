/*  Общая модель данных таблиц.
*/
#ifndef STABLEBASEMODEL_H
#define STABLEBASEMODEL_H

#include <QSqlQueryModel>
#include <QObject>
#include <QDebug>

class STableBaseModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit STableBaseModel(QObject *parent = nullptr);
    int field(const QString &field);
    QDateTime timestampUtc(const QModelIndex &index) const;
    QDateTime timestampLocal(const QModelIndex &index) const;
    double total(int column, int sign = 0);
protected:
    QMap<QString, int> i_fields;
private slots:
    void cashFieldsNames();
};

#endif // STABLEBASEMODEL_H
