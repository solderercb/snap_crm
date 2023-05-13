#ifndef STABLESALARYRECEPTEDISSUED_H
#define STABLESALARYRECEPTEDISSUED_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryReceptedIssued : public STableBaseModel
{
    Q_OBJECT
public:
    enum IncludePayedInTotal {IncludePayed = 0, ExcludePayed = 1};
    explicit STableSalaryReceptedIssued(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYRECEPTEDISSUED_H
