#ifndef STABLETECHREPORTSMODEL_H
#define STABLETECHREPORTSMODEL_H

#include <STableModelBase>

class STableTechReportsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns{Id = 0, Num, Created, User, Company, Client, Device, InvNumber, SerialNumber, ProductionDate, PurchaseDate, InitCost, ResidualCost, Fault, DiagResult, Conclusion, Notes, ClientShortName};
    explicit STableTechReportsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//    Qt::ItemFlags flags(const QModelIndex& index) const override;
private:
    QVariant clientName(const QModelIndex &index) const;
    QVariant deviceDate(const QModelIndex &index) const;
    QModelIndex indexForShortData(const QModelIndex &index) const override;
};

#endif // STABLETECHREPORTSMODEL_H
