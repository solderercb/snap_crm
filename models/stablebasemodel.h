/*  Общая модель данных таблиц.
*/
#ifndef STABLEBASEMODEL_H
#define STABLEBASEMODEL_H

#include <QSqlQueryModel>
#include "stablemodelscommonmethods.h"
#include <QObject>
#include <QDebug>

class STableBaseModel : public QSqlQueryModel, public STableModelsCommonMethods
{
    Q_OBJECT
public:
    explicit STableBaseModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
private slots:
    void slotModelReset();
};

#endif // STABLEBASEMODEL_H
