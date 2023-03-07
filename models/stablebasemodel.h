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

};

#endif // STABLEBASEMODEL_H
