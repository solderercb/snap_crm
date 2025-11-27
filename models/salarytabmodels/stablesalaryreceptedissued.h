#ifndef STABLESALARYRECEPTEDISSUED_H
#define STABLESALARYRECEPTEDISSUED_H

#include <STableModelBase>

class STableSalaryReceptedIssued : public STableBaseModel
{
    Q_OBJECT
public:
    enum IncludePayedInTotal {IncludePayed = 0, ExcludePayed = 1};
    enum Columns {Id = 0, Title = 1, RealRepairCost = 2, InDate = 3, PayDeviceIn = 4, PayDeviceOut = 4};
    explicit STableSalaryReceptedIssued(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYRECEPTEDISSUED_H
