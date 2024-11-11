#ifndef SPARTSREQUESTSMODEL_H
#define SPARTSREQUESTSMODEL_H

#include "models/seditablebasemodel.h"
#include "models/sstandarditemmodel.h"
#include "modules/purchasemanager/srequest.h"
#include <QSqlIndex>
#include <QObject>

class SPartsRequestsModel : public SEditableBaseModel
{
    Q_OBJECT
signals:
    void stateChanged(const SPartRequest::State state);
public:
    enum Columns{Check = 0, Id = 1, Created, Employee, Client, Name, Count, Dealer, Repair, State, Tracking, Priority, Progress, EndDate, PlanEndDate, Summ, ItemId, Notes, Url, ClientShortName};
    Q_ENUM(Columns)
    explicit SPartsRequestsModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~SPartsRequestsModel();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &item) const override;
    Qt::ItemFlags flagsMask(const QModelIndex &item) const;
    bool removeRow(const int row, const QModelIndex &parent = QModelIndex());
    void clear() override;
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    void setReadOnly(bool state);
private:
    QList<QModelIndex> m_itemsToInvoice;
    bool m_modelRO = 0;
    void translateNames();
    QVariant clientName(const QModelIndex &index) const;
    QVariant dateTime(const QModelIndex &index) const;
    bool toggleCheckBox(const QModelIndex &index);
    int checkBoxState(const QModelIndex &index) const;
    bool updateRowInTable(int row, const QSqlRecord &values) override;
    QModelIndex indexForShortData(const QModelIndex &index) const override;
public slots:
    bool commit();
    bool select() override;
    bool selectRow(int row) override;
};

#endif // SPARTSREQUESTSMODEL_H
