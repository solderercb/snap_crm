#ifndef STABLESALARYSALESMODEL_H
#define STABLESALARYSALESMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalarySalesModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalarySalesModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYSALESMODEL_H
