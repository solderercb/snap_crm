#ifndef SSALETABLEMODEL_H
#define SSALETABLEMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QMap>
#ifdef QT_DEBUG
#include <QRandomGenerator>
#endif
#include "../global.h"
#include "../com_sql_queries.h"
#include "models/sstoresaleitemmodel.h"
#include "models/srepairsaleitemmodel.h"
#include "models/sworkmodel.h"

class SSaleTableModel : public QStandardItemModel

{
    Q_OBJECT
public:
    enum State {StoreNew = 0, StoreSold = 1, StoreReserved = 2, StoreCancelled = 3, WorkshopRW = 4, WorkshopRO = 5};
    enum BackOutOpType {FreeReserved = 0, Unsale = 1};
    enum SaleOpType {Reserve = 0, Sale = 1, Repair = 2};
    enum class RepairOpType { Nop = 0, Sale = 1, Update = 2, Unsale = 3, Free = 4 };
    enum DataRoles {QtyBackup = Qt::UserRole, Changed = Qt::UserRole + 1, State = Qt::UserRole + 2, RecordType = Qt::UserRole + 3};
    enum RecordType {Item = 0, Work = 1};
    enum TablesSet {StoreSale = 0, WorkshopSale = 1};
    enum EditStrategy {OnFieldChange = 0, OnRowChange = 1, OnManualSubmit = 2};
    explicit SSaleTableModel(QObject *parent = nullptr);
    ~SSaleTableModel();
    QVariant value(const int, const int, const int role = Qt::DisplayRole) const;
    void setModelState(int state) {m_modelState = state;};
    int modelState() {return m_modelState;};
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    bool insertRecord(int, const QSqlRecord&, const int recType = RecordType::Item);
    bool appendRecord(const QSqlRecord&);
    void removeRowHandler(const int, const int);
    void storeSaleRemoveRow(const int, const int);
    void workshopSaleRemoveRow(const int, const int);
    QMap<int, int>* getPendingRemoveList();
    int pendingRemoveItemsCount();
    void setHorizontalHeaderLabels(const QStringList &labels);
    double amountTotal();
    QString amountTotalLocale();
    QString amountItemsLocale();
    QString amountWorksLocale();
    QMap<QString, int> *fields;
    void setPriceColumn(QSqlQuery*);
    bool loadStoreSale(const int);
    bool loadWorkshopSale(const int);
    void setClient(int);
    void unsetClient();
    void setDocumentId(int);
    void setRepairId(int);
    void setDocumentState(int state){m_documentState = state;};
    bool commit();
    bool saveTablesStore(SaleOpType type = SaleOpType::Sale);
    bool saveTablesWorkshop(RepairOpType operation = RepairOpType::Update);
    bool reserveItems();
    bool unsaleItems();
    bool unsaleItems(const QString&);
    void setUnsaleReason(const QString&);
    bool freeItems();
    bool storeBackOutItems(BackOutOpType);
    bool workshopBackOutItems(BackOutOpType);
    int itemsAffected();
    void markAllItemsToRemove(BackOutOpType);
    SStoreSaleItemModel* storeItem(const int);
    SRepairSaleItemModel* repairItem(const int);
    SWorkModel* repairWork(const int);
    bool integrityStatus();
    int tableMode();
    void setTableMode(const TablesSet mode = TablesSet::StoreSale);
    bool saleRepairItems();
    bool isColumnHidden(const int);
    int editStrategy();
    void clearChangedFlags();

//    void setDocument(int);

signals:
    void dataChanged();
    void amountChanged(float, float, float);
    void modelReset();

private:
    QSqlQueryModel *m_queryData;
    int m_modelState = 0; // 0 - новая РН (всё редактируется, строки удаляются); 1 - проведённая РН (не редактируется, строки помечаются серым); 2 - резерв (всё редактируется, строки помечаются серым); 3 - распроведённая РН или снятый резерв (не редактируется, кнопок в первом столбце нет)
    int m_documentId = 0;
    int m_repairId = 0;
    int m_documentState = 0;
    int m_client = 0;
    int m_itemsAffected = 0;
    QMap<int, int> *m_pendingRemoveList;
    QString m_priceFieldName = "`price2`";
    bool m_amountChangedSignalFilter = 0;
     bool m_nIntegrityErr = 1;
     QString m_unsaleReason;
     bool m_tableMode = TablesSet::StoreSale;
     int m_editStrategy = EditStrategy::OnFieldChange;
     float m_amountItems = 0, m_amountWorks = 0, m_amountTotal = 0;
     QList<QStandardItem *> row(int) const;

    // названия столбцов по-умолчанию; подробнее см. в комментарии к методу SaleTableModel::setHorizontalHeaderLabels
    QStringList m_fieldsDep = {"id", "UID", "name", "count", "avail", "price", "summ", "box", "sn", "warranty", "user", "is_realization", "return_percent", "state", "notes", "item_id", "in_price", "obj_id", "dealer", "buyer", "created", "work_id", "is_item"};
    int m_hiddenColumns = 0x00000000;

private slots:
    void sqlDataChanged();
#if QT_VERSION >= 0x060000
    void dataChanaged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>());
#else
    void dataChanaged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>());
#endif

};

#endif // SSALETABLEMODEL_H
