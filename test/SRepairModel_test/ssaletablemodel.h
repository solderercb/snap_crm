#ifndef SSALETABLEMODEL_H
#define SSALETABLEMODEL_H

#include <QObject>
#include <QStandardItem>
#include "../../models/ssinglerowjmodel.h"

class SSaleTableModel : public QObject
{
public:
    enum State {StoreNew = 0, StoreSold = 1, StoreReserved = 2, StoreCancelled = 3, WorkshopRW = 4, WorkshopRO = 5, WorkshopAdm = 6};
    explicit SSaleTableModel(QObject *parent = nullptr);
    ~SSaleTableModel();
    int state(){return SSaleTableModel::State::WorkshopRW;};
};

#endif // SSALETABLEMODEL_H
