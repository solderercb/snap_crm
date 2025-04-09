#ifndef SPARTSUPPLIERSMODEL_H
#define SPARTSUPPLIERSMODEL_H

#include "modules/purchasemanager/srequest.h"
#include "modules/purchasemanager/sgroupingmodel.h"
#include "models/srelationalbasemodel.h"
#include <QObject>
#include <QSqlField>

class SPartSuppliersModel : public SRelationalBaseModel
{
    Q_OBJECT
public:
    enum Columns{Id = 0, RequestId, Supplier, SupplierUrl, ItemUrl, Moq, Price, Select, Notes};
    Q_ENUM(Columns)
    enum PostSubmitAction{DefaultSelect = -1, NoSelect = 0, SelectById = 1};
    explicit SPartSuppliersModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~SPartSuppliersModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    bool appendRow();
    void updateFilter(const int id = 0);
    bool select() override;
    bool select(const int requestId);
    int requestId() const;
    void setRequestId(int requestId);
    void setRequestState(int state);
    bool updateRowInTable(int row, const QSqlRecord &values) override;
    bool insertRowIntoTable(const QSqlRecord &values) override;
    bool deleteRowFromTable(int row) override;
    bool isDirty() const;
    bool submit() override;
    void submitAll();
    int predefSupplierId();
    void setPredefSupplierId(const int id);
    void addSupplierRecord(int id);
    void setRowHighlightingClause(const int id, const QString &name);
private:
    int m_requestId = 0;
    int m_requestState = SPartRequest::Created;
    bool m_itemsEditableFlagOverride = 0;
    int m_postSubmitAction = PostSubmitAction::DefaultSelect;
    int m_newCheckedId = 0;
    int m_predefSupplierId = 0;
    bool m_highlightRows = 0;
    int m_highlightId = -1;
    QString m_highlightName = QString();
    mutable int m_highlightedRow = -1;
    QVariant supplierName(const QModelIndex &index) const;
    QString url(const QModelIndex &index) const;
    bool setCheckStateExclusive(const QModelIndex &checkedIndex);
    void slotPrimeInsert(int row, QSqlRecord &record);
    bool postSubmitDispatcher(const bool &queryResult);
};

#endif // SPARTSUPPLIERSMODEL_H
