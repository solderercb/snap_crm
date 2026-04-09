#ifndef STABLECASHBOXMODEL_H
#define STABLECASHBOXMODEL_H

#include <STableModelBase>

class SPaymentTypesModel;

class STableCashboxModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum PaymentSystem {ByCash = 0, Cashless = 1, ByCard = 2, Other = 3};
    enum Columns {Id = 0, Created = 1, Office = 2, AmountsByCash = 3, AmountsCashless = 4, AmountsByCard = 5,
                  AmountsOtherType = 6, PaymentType = 7, Covenantor = 8, Employee = 9, Reason = 10, IsBackdate = 11, ShortName = 12};
    Q_ENUM(Columns)
    explicit STableCashboxModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    int id(const int row);
    double total(int column, int direction);
protected:
    QString dataLocalizedFromDouble(const QModelIndex &item) const;
private:
    std::shared_ptr<SPaymentTypesModel> m_allOrdersTypes;
    QModelIndex indexForShortData(const QModelIndex &index) const override;
};

#endif // STABLECASHBOXMODEL_H
