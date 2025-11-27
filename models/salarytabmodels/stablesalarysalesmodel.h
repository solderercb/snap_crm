#ifndef STABLESALARYSALESMODEL_H
#define STABLESALARYSALESMODEL_H

#include <STableModelBase>

class STableSalarySalesModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns {Id = 0, Created = 1, Client = 2, Amount = 3, Profit = 4, SalaryPart = 5};
    explicit STableSalarySalesModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYSALESMODEL_H
