#ifndef STABLECLIENTSMODEL_H
#define STABLECLIENTSMODEL_H

#include "stablebasemodel.h"
#include <QObject>

class STableClientsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns {Id = 0, FullName = 1, Balance = 2, Repairs = 3, Purchases = 4, Type = 5, Phone = 6, EMail = 7, State = 8, ShortName = 9};
    Q_ENUM(Columns)
    explicit STableClientsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    QModelIndex indexForShortData(const QModelIndex &index) const override;
};

#endif // STABLECLIENTSMODEL_H
