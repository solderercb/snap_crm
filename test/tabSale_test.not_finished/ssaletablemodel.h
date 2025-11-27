#ifndef SSALETABLEMODEL_H
#define SSALETABLEMODEL_H

#include <QObject>
#include <QStandardItem>
#include <sstoreitemmodel>
#include "../../models/ssinglerowjmodel.h"

class SSaleTableModel : public QAbstractItemModel
{
public:
    enum State {StoreNew = 0, StoreSold = 1, StoreReserved = 2, StoreCancelled = 3, WorkshopRW = 4, WorkshopRO = 5, WorkshopAdm = 6};
    enum TablesSet {StoreSale = 0, WorkshopSale = 1};
    enum class StoreOpType {Reserve = 0, Sale, Unsale, FreeReserved};
    explicit SSaleTableModel(QObject *parent = nullptr);
    ~SSaleTableModel();
    void setMode(const TablesSet mode = TablesSet::StoreSale);
    void setState(int state);
    int state(){return SSaleTableModel::State::WorkshopRW;};
    bool store_loadTable(const int){return 1;};
    QString amountTotalLocale(){return "1 024,00 ₽";};
    QVariant value(const int, const int, const int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
    void setClient(int);
    void unsetClient();
    void setDocumentId(int);
    void setPriceColumn(const SStoreItemModel::PriceOption id);
    bool addItemByUID(int){return 1;};
    void unsaleItems(const QString&);
    int itemsAffected(){return 1;};
    double amountTotal(){return 1024;};
    bool store_saveTables(StoreOpType type = StoreOpType::Sale);
    bool freeItems();
    void clearChangedFlagForAllField();
    void dbgAddRandomItem();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

QModelIndex SSaleTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex SSaleTableModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int SSaleTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int SSaleTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SSaleTableModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

#endif // SSALETABLEMODEL_H
