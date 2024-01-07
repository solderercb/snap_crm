#ifndef STABLETECHREPORTSMODEL_H
#define STABLETECHREPORTSMODEL_H

#include <QObject>
#include "models/stablebasemodel.h"
#include "global.h"

class STableTechReportsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns{Id = 0, Num, Created, User, Company, Client, Device, InvNumber, SerialNumber, ProductionData, PurchaseDate, InitCost, ResidualCost, Fault, DiagResult, Conclusion, Notes};
    explicit STableTechReportsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//    Qt::ItemFlags flags(const QModelIndex& index) const override;
private:
    QVariant clientName(const QModelIndex &index) const;
    QVariant dateTime(const QModelIndex &index) const;
};

#endif // STABLETECHREPORTSMODEL_H
