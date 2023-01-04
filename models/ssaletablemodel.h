#ifndef SSALETABLEMODEL_H
#define SSALETABLEMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QMap>
#ifdef QT_DEBUG
#include <QRandomGenerator>
#endif
#include "squerylog.h"
#include "../global.h"
#include "../com_sql_queries.h"
#include "models/sstoresaleitemmodel.h"
#include "models/srepairsaleitemmodel.h"
#include "models/sworkmodel.h"

class SSaleTableModel : public QStandardItemModel

{
    Q_OBJECT
public:
    enum State {StoreNew = 0, StoreSold = 1, StoreReserved = 2, StoreCancelled = 3, WorkshopRW = 4, WorkshopRO = 5, WorkshopAdm = 6};
    enum class StoreOpType {Reserve = 0, Sale, Unsale, FreeReserved};
    enum class RepairOpType {Reserve = 0, Link, Sale, Unsale, Unlink, Update};
    enum DataRoles {OldValue = Qt::UserRole, Changed = Qt::UserRole + 1, State = Qt::UserRole + 2, RecordType = Qt::UserRole + 3};
    enum RecordType {Work = 0, Item = 1};
    enum TablesSet {StoreSale = 0, WorkshopSale = 1};
    enum EditStrategy {OnFieldChange = 0, OnRowChange = 1, OnManualSubmit = 2, Nop = 0x55AA};
    explicit SSaleTableModel(QObject *parent = nullptr);
    ~SSaleTableModel();
    QVariant value(const int, const int, const int role = Qt::DisplayRole) const;
    void setModelState(int state);
    int modelState();
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    bool insertRecord(int, const QSqlRecord &, const int recType = RecordType::Item);
    bool appendRecord(const QSqlRecord&);
    int isItemAlreadyInList(int);
    bool addWorkByUID(const int uid, const int priceOption);
    bool addItemByUID(const int uid, const int priceOption, const int count = 1);
    bool addItemFromBasket(const int id, const int qty = 0, const int priceOpt = 0);
    void insertrepair_item(const int);
    void removeRowHandler(const int, const int);
    void buttonHandler(const int buttonNum, const int row);
    void store_markRowRemove(const int, const int);
    int repair_markRowRemove(const int, const int);
    QMap<int, int>* getPendingRemoveList();
    int pendingRemoveItemsCount();
    void setHorizontalHeaderLabels();
    double amountTotal();
    QString amountTotalLocale();
    double amountItems();
    QString amountItemsLocale();
    QString amountWorksLocale();
    QMap<QString, int> *fields;
    void setPriceColumn(const int index);
    bool store_loadTable(const int);
    bool repair_loadTable(const int);
    void setClient(int);
    void unsetClient();
    void setDocumentId(int);
    void setRepairId(int);
    void setDocumentState(int state){m_documentState = state;};
    bool commit();
    bool store_saveTables(StoreOpType type = StoreOpType::Sale);
    bool repair_saveTablesStandalone();
    bool repair_saveTables();
    bool repair_saveTables(RepairOpType operation);
    bool repair_autoSaveTables();
    bool reserveItems();
    bool unsaleItems();
    bool unsaleItems(const QString&);
    void setExtraUnsaleReason(const QString&);
    bool freeItems();
    bool store_backOutItems(StoreOpType);
    bool isRowMarkedRemove(const int) const;
    bool repair_removeRows();
    bool repair_removeItems();
    bool repair_removeWorks();
    void sale();
    int itemsAffected();
    void store_markAllItemsToRemove(StoreOpType);
    SStoreSaleItemModel* store_item(const int);
    SRepairSaleItemModel* repair_item(const int);
    SWorkModel* repair_work(const int);
    bool integrityStatus();
    int tableMode();
    void setTableMode(const TablesSet mode = TablesSet::StoreSale);
    bool isColumnHidden(const int);
    int editStrategy();
    void setEditStrategy(const int);
    bool isWarranty();
    void setIsWarranty(const bool);
    bool isUnsaved();
#ifdef QT_DEBUG
    void dbgAddRandomItem();
    void dbgAddRandomItemBasket();
#endif

signals:
    void tableDataChanged();
    void tableSaved();
    void amountChanged(double, double, double);
    void modelStateChanged(int);
    void addItem();

private:
    QSqlQueryModel *m_queryData;
    int m_modelState = 0; // 0 - новая РН (всё редактируется, строки удаляются); 1 - проведённая РН (не редактируется, строки помечаются серым); 2 - резерв (всё редактируется, строки помечаются серым); 3 - распроведённая РН или снятый резерв (не редактируется, кнопок в первом столбце нет)
    int m_objId = 0;
    int m_documentState = 0;
    int m_client = 0;
    int m_itemsAffected = 0;
    QMap<int, int> *m_itemsPendingRemoveList;
    QMap<int, int> *m_worksPendingRemoveList;
    QMap<int, int> *m_itemsPendingSplitList;
    int m_priceIndex = 1; // по умолчанию "Цена розница" ("`price2`")
    bool m_amountChangedSignalFilter = 0;
    QString m_extraUnsaleReason;
    bool m_tableMode = TablesSet::StoreSale;
    int m_editStrategy = EditStrategy::OnFieldChange;
    double m_amountItems = 0, m_amountWorks = 0, m_amountTotal = 0;
    int m_currentIndex = -1;
    int m_lastHandledWorkId = -1;
    bool m_isWarranty = 0;
    bool m_unsaved = 0;
    QList<QStandardItem *> row(int) const;

    // названия столбцов по-умолчанию; подробнее см. в комментарии к методу SaleTableModel::setHorizontalHeaderLabels
    QStringList m_fieldsDep = {"id", "UID", "name", "count", "avail", "price", "summ", "box", "sn", "warranty", "user", "is_realization", "return_percent", "state", "notes", "item_id", "in_price", "obj_id", "dealer", "buyer", "created", "work_id", "is_item"};
    int m_hiddenColumns = 0x00000000;
    int getItemInsertionRow();
    int getParentWorkRow(const int itemRow);
    bool recordType(const int row);
    void linkItemToWork(const int row, const int workId);
    void clearChangedFlagForAllField();
    int activeRowCount() const;

public slots:
    void addCustomWork();
    void indexSelected(const QModelIndex &index);

private slots:
    void sqlDataChanged();
#if QT_VERSION >= 0x060000
    void dataChangedHook(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>());
#else
    void dataChangedHook(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>());
#endif

};

#endif // SSALETABLEMODEL_H
