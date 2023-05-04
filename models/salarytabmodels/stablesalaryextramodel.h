#ifndef STABLESALARYEXTRAMODEL_H
#define STABLESALARYEXTRAMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryExtraModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryExtraModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYEXTRAMODEL_H
