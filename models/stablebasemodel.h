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
protected:
    QMap<QString, int> i_fields;
    void beginResetModel();
    void endResetModel();
private slots:
    void cashFieldsNames();
};

#endif // STABLEBASEMODEL_H
