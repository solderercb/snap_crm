#ifndef STABLECLIENTSMODEL_H
#define STABLECLIENTSMODEL_H

#include "stablebasemodel.h"
#include <QObject>

class STableClientsModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableClientsModel(QObject *parent = nullptr);
};

#endif // STABLECLIENTSMODEL_H
