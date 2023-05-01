#ifndef STABLESALARYSALEPARTSMODEL_H
#define STABLESALARYSALEPARTSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalarySalePartsModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalarySalePartsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYSALEPARTSMODEL_H
