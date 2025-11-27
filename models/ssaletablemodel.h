#ifndef SSALETABLEMODEL_H
#define SSALETABLEMODEL_H

#include <SSingleRowJModel>
#include <SStandardItemModel>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QMap>
#include <SStoreItemModel>
#include <SStoreSaleItemModel>
#include <SRepairSaleItemModel>
#include <SWorkModel>
#include <STableModelCommonMethods>

class QSqlQueryModel;
class SStoreSaleItemModel;
class SRepairSaleItemModel;
class SWorkModel;
class SCartridgeCardModel;

class SSaleTableModel : public SStandardItemModel, public STableModelsCommonMethods
{
    Q_OBJECT
    Q_PROPERTY(QString UID READ reportUID)
    Q_PROPERTY(QString id READ reportId)
    Q_PROPERTY(QString item READ reportItem)
    Q_PROPERTY(QString qty READ reportQty)
    Q_PROPERTY(QString price READ reportPrice)
    Q_PROPERTY(QString summ READ reportSumm)
    Q_PROPERTY(QString serialNumber READ reportSN)
    Q_PROPERTY(QString warranty READ reportWarranty)
    Q_PROPERTY(QString performer READ reportPerformer)
    Q_PROPERTY(QString worksAmount READ amountWorksLocale)
    Q_PROPERTY(QString itemsAmount READ amountItemsLocale)
    Q_PROPERTY(double amount READ amountTotal)
    Q_PROPERTY(QString amountStr READ amountTotalLocale)

    friend class tabSale;
    friend class STableViewBOQItemDelegates;
    friend class TClassTest;
public:
    enum RecordType {Work = 0, Item = 1};
    enum EditStrategy {OnFieldChange = 0, OnRowChange = 1, OnManualSubmit = 2, Nop = 0x55AA};
    enum AddItemException {AlreadyAdded = 0, NotAvailable = 1, ForeignWarehouse = 2, NotLinked = 3};
    enum Columns {Id = 0, UID, Name, Count, Avail, Price, Amount, Box, SN, Warranty, User, Realization, RetPercent, State, Notes, ItemId, InPrice, ObjId, Dealer, Buyer, Created, WorkId, RecordType, WorkType};
    Q_ENUM(Columns)
    explicit SSaleTableModel(QObject *parent = nullptr);
    ~SSaleTableModel();
    void setState(int state);
    int state() const;
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex&) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int recordType(const int row) const;
    void setRecordType(const int row, const int type);
    int workType(const int row) const;
    void flashPopupInfoMessage(const QString &caption, const QString &text);
    virtual bool checkBeforeItemAdd(const int id, const int count);
    bool addWorkByID(const int id, const int priceOption);
    bool addItemByID(const int id, const int count = 1);
    bool addItemByID(const int id, const SStoreItemModel::PriceOption priceOption, const int count = 1);
    virtual void removeRow(const int row) = 0;
    virtual void markRowRemove(const int row) = 0;
    void endRemoveRow();
    void setHorizontalHeaderLabels();
    void rowAmountUpdate(const QModelIndex &index);
    void calculateAmounts();
    double amountItems();
    QString amountItemsLocale();
    QString amountWorksLocale();
    double amountTotal();
    QString amountTotalLocale();
    void setPriceColumn(const SStoreItemModel::PriceOption id);
    virtual bool loadTable(const int doc_id) = 0;
    void setClient(int);
    void unsetClient();
    void setDocumentId(int);
    void setRepairId(int);
    virtual bool commit(int operation) = 0;
    virtual bool endModelChange() = 0;
    virtual bool isRowMarkedRemove(const int) const = 0;
    int itemsAffected();
    virtual bool isModelEditable() const = 0;
    int visibleColumnIndex(const int);
    virtual int pixmapFlags(const QModelIndex&) const;
    virtual void initDemo();
    QString reportUID();
    QString reportId();
    QString reportItem();
    QString reportQty();
    QString reportPrice();
    QString reportSumm();
    QString reportSN();
    QString reportWarranty();
    QString reportPerformer();
    QMap<int, int> i_columnWidths;  // не используется; условная заглушка для нормальной работы STableViewBase с этой моделью данных
    QSqlDatabase loadConnection() const;
    QSqlDatabase commitConnection() const;
    void setConnection(const QSqlDatabase &conn);
    bool isWarranty();
    void setIsWarranty(const bool);
    void clear();
#if defined QT_DEBUG || defined S_TEST
    int dbgAddRandomItems(const int qty);
#endif
signals:
    void tableDataChanged();
    void tableSaved();
    void amountChanged(double, double, double);
protected:
    QMap<QString, int> *fields;
    std::shared_ptr<QSqlDatabase> m_connection;
    QSqlQueryModel *m_queryData;
    int m_modelState = 0;
    int m_objId = 0;
    int m_client = 0;
    bool m_isWarranty = 0;
    int m_itemsAffected = 0;
    QMap<SSingleRowJModel*, std::shared_ptr<SSingleRowJModel>> m_recordsPendingRemoveMap;   // сырой указатель используется в качестве ключа
    QMap<SSingleRowJModel*, std::shared_ptr<SSingleRowJModel>> m_itemsPendingSplitMap;
    SStoreItemModel::PriceOption m_priceColumnId = SStoreItemModel::PriceOptionRetail;
    double m_amountItems = 0, m_amountWorks = 0, m_amountTotal = 0;
    int m_currentIndex = -1;
    qsizetype m_reportRowNum = 0;
    // названия столбцов по-умолчанию; подробнее см. в комментарии к методу SaleTableModel::setHorizontalHeaderLabels
//    QStringList m_fieldsDep = {"id", "UID", "name", "count", "avail", "price", "summ", "box", "sn", "warranty", "user", "is_realization", "return_percent", "state", "notes", "item_id", "in_price", "obj_id", "dealer", "buyer", "created", "work_id", "is_item", "work_type"};
    int m_hiddenColumns = 0x000FF810;
    virtual int itemInsertionRow() = 0;
    int getParentWorkRow(const int itemRow);
    int activeRowCount() const;
    std::shared_ptr<SSingleRowJModel> singleRowModel(const int row) override;
    bool initSingleRowModel(const int row, std::shared_ptr<SSingleRowJModel> model);
    bool setSingleRowModelData(const QSqlRecord &record, std::shared_ptr<SSingleRowJModel> model);
    virtual void itemAddHook(const int){};
    void addToPendingRemoveList(std::shared_ptr<SSingleRowJModel> model);
    virtual int saleMode() const = 0;
    int itemId(const int row) const;
    QString name(const int row) const;
    double price(const int row) const;
    int count(const int row) const;
    double amountItem(const int row) const;
    int state(const int row) const;
public slots:
    void addCustomWork();
    void indexSelected(const QModelIndex &index);
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
private slots:
    void sqlDataChanged();
};

inline int SSaleTableModel::itemId(const int row) const
{
    return SStandardItemModel::data(index(row, Columns::ItemId)).toInt();
}

inline QString SSaleTableModel::name(const int row) const
{
    return SStandardItemModel::data(index(row, Columns::Name)).toString();
}

inline double SSaleTableModel::price(const int row) const
{
    return SStandardItemModel::data(index(row, Columns::Price)).toDouble();
}

inline int SSaleTableModel::count(const int row) const
{
    return SStandardItemModel::data(index(row, Columns::Count)).toInt();
}

inline int SSaleTableModel::state(const int row) const
{
    return SStandardItemModel::data(index(row, Columns::State)).toInt();
}

inline int SSaleTableModel::recordType(const int row) const
{
    return QStandardItemModel::data(index(row, Columns::RecordType)).toInt();
}

/**************************************************************************************/
class RecordFactory {
public:
    using FactoryFunc = std::function<std::shared_ptr<SSingleRowJModel>()>;

    static RecordFactory& instance() {
        static RecordFactory inst;
        return inst;
    }

    void registerFactory(const int type, FactoryFunc func) {
        factories_[type] = std::move(func);
    }

    std::shared_ptr<SSingleRowJModel> create(const int type) const {
        if (auto it = factories_.find(type); it != factories_.end())
            return it->second();
        return nullptr;
    }
private:
    std::unordered_map<int, FactoryFunc> factories_;
};

template <typename T>
struct TypeFabricRegister {
    TypeFabricRegister(const int type) {
        RecordFactory::instance().registerFactory(type, [] {
            return std::make_shared<T>();
        });
    }
};

class SStoreItemModelMapper
{
public:
    static int srm_index(const int tableModelIndex) {
        switch (tableModelIndex) {
//            case SSaleTableModel::Columns::Count: return SStoreItemModel::C_count;
            case SSaleTableModel::Columns::Price: return SStoreItemModel::C_price;
            case SSaleTableModel::Columns::Realization: return SStoreItemModel::C_isRealization;
            case SSaleTableModel::Columns::RetPercent: return SStoreItemModel::C_returnPercent;
            case SSaleTableModel::Columns::ItemId: return SStoreItemModel::C_id;
            case SSaleTableModel::Columns::InPrice: return SStoreItemModel::C_inPrice;
            case SSaleTableModel::Columns::Dealer: return SStoreItemModel::C_dealer;
            default: return -1;
        }
    };
    static bool setField(std::shared_ptr<SStoreItemModel> model, const int index, const QVariant &data){
        bool ret = 1;
        switch(index) {
            case SSaleTableModel::Columns::ItemId: model->setPrimaryKey(data.toInt()); break;
            case SSaleTableModel::Columns::Count: model->setNewExpense(data.toInt()); break;
            case SSaleTableModel::Columns::ObjId: model->setSaleEntityId(data.toInt()); break;
            default: model->initFieldWithPrevLoaded(srm_index(index), data);    // данные копируются в m_record
        }
        return ret;
    };
    static bool initField(std::shared_ptr<SStoreItemModel> model, const int index, const QVariant &data){
        bool ret = 1;
        switch(index) {
            case SSaleTableModel::Columns::Count: model->setCommitedExpense(data.toInt()); Q_FALLTHROUGH();
            default: ret &= setField(model, index, data);
        }
        return ret;
    };
};

class SStoreSaleItemModelExt : public SStoreSaleItemModel, public MapperForSingleRowModel
{
public:
    SStoreSaleItemModelExt() : SStoreSaleItemModel(){};
    int singleRowModelIndex(const int tableModelIndex) override
    {
        switch (tableModelIndex)
        {
            case SSaleTableModel::Columns::Id: return SStoreSaleItemModel::C_id;
            case SSaleTableModel::Columns::Count: return SStoreSaleItemModel::C_count;
            case SSaleTableModel::Columns::Price: return SStoreSaleItemModel::C_price;
            case SSaleTableModel::Columns::SN: return SStoreSaleItemModel::C_sn;
            case SSaleTableModel::Columns::Warranty: return SStoreSaleItemModel::C_warranty;
            case SSaleTableModel::Columns::User: return SStoreSaleItemModel::C_user;
            case SSaleTableModel::Columns::Realization: return SStoreSaleItemModel::C_isRealization;
            case SSaleTableModel::Columns::RetPercent: return SStoreSaleItemModel::C_returnPercent;
            case SSaleTableModel::Columns::State: return SStoreSaleItemModel::C_state;
            case SSaleTableModel::Columns::ItemId: return SStoreSaleItemModel::C_itemId;
            case SSaleTableModel::Columns::InPrice: return SStoreSaleItemModel::C_inPrice;
            case SSaleTableModel::Columns::ObjId: return SStoreSaleItemModel::C_documentId;
            case SSaleTableModel::Columns::Dealer: return SStoreSaleItemModel::C_dealer;
            case SSaleTableModel::Columns::Buyer: return SStoreSaleItemModel::C_buyer;
            default: return -1;
        }
    }
    bool setSingleRowModelData(const int index, const QVariant &data) override {
        SStoreItemModelMapper::setField(m_storeItem, index, data);
        auto srm_index = setExtraData(index, data);
        if(!srm_index.has_value())
            return 1;   // данные были записаны в контейнер m_extraFields

        return SStoreSaleItemModel::setData(srm_index.value(), data);
    };
    void initSingleRowModelField(const int index, const QVariant &data) override {
        SStoreItemModelMapper::initField(m_storeItem, index, data);
        auto srm_index = setExtraData(index, data);
        if(!srm_index.has_value())
            return;     // данные были записаны в контейнер m_extraFields
        initFieldWithPrevLoaded(srm_index.value(), data);
    };
};

class SRepairSaleItemModelExt : public SRepairSaleItemModel, public MapperForSingleRowModel
{
public:
    SRepairSaleItemModelExt() : SRepairSaleItemModel(){};
    int singleRowModelIndex(const int tableModelIndex) override
    {
        switch (tableModelIndex)
        {
            case SSaleTableModel::Columns::Id: return SRepairSaleItemModel::C_id;
            case SSaleTableModel::Columns::Name: return SRepairSaleItemModel::C_name;
            case SSaleTableModel::Columns::Count: return SRepairSaleItemModel::C_count;
            case SSaleTableModel::Columns::Price: return SRepairSaleItemModel::C_price;
            case SSaleTableModel::Columns::SN: return SRepairSaleItemModel::C_sn;
            case SSaleTableModel::Columns::Warranty: return SRepairSaleItemModel::C_warranty;
            case SSaleTableModel::Columns::User: return SRepairSaleItemModel::C_toUser;
            case SSaleTableModel::Columns::State: return SRepairSaleItemModel::C_state;
            case SSaleTableModel::Columns::Notes: return SRepairSaleItemModel::C_notes;
            case SSaleTableModel::Columns::ItemId: return SRepairSaleItemModel::C_itemId;
            case SSaleTableModel::Columns::ObjId: return SRepairSaleItemModel::C_repairId;
            case SSaleTableModel::Columns::Created: return SRepairSaleItemModel::C_created;
            case SSaleTableModel::Columns::WorkId: return SRepairSaleItemModel::C_workId;
            default: return -1;
        }
    }
    bool setSingleRowModelData(const int index, const QVariant &data) override {
        SStoreItemModelMapper::setField(m_storeItem, index, data);
        auto srm_index = setExtraData(index, data);
        if(!srm_index.has_value())
            return 1;   // данные были записаны в контейнер m_extraFields
\
        return SRepairSaleItemModel::setData(srm_index.value(), data);
    };
    void initSingleRowModelField(const int index, const QVariant &data) override {
        SStoreItemModelMapper::initField(m_storeItem, index, data);
        auto srm_index = setExtraData(index, data);
        if(!srm_index.has_value())
            return;     // данные были записаны в контейнер m_extraFields
        initFieldWithPrevLoaded(srm_index.value(), data);
    };
};

class SWorkModelExt : public SWorkModel, public MapperForSingleRowModel
{
public:
    SWorkModelExt() : SWorkModel(){};
    int singleRowModelIndex(const int tableModelIndex) override
    {
        switch (tableModelIndex)
        {
            case SSaleTableModel::Columns::Id: return SWorkModel::C_id;
            case SSaleTableModel::Columns::Name: return SWorkModel::C_name;
            case SSaleTableModel::Columns::Count: return SWorkModel::C_count;
            case SSaleTableModel::Columns::Price: return SWorkModel::C_price;
            case SSaleTableModel::Columns::Warranty: return SWorkModel::C_warranty;
            case SSaleTableModel::Columns::User: return SWorkModel::C_user;
            case SSaleTableModel::Columns::RetPercent: return SWorkModel::C_payRepair;
            case SSaleTableModel::Columns::ItemId: return SWorkModel::C_priceId;
            case SSaleTableModel::Columns::ObjId: return SWorkModel::C_repair;
            case SSaleTableModel::Columns::Created: return SWorkModel::C_created;
            case SSaleTableModel::Columns::WorkType: return SWorkModel::C_type;
            default: return -1;
        }
    }
    bool setSingleRowModelData(const int index, const QVariant &data) override {
        auto srm_index = setExtraData(index, data);
        if(!srm_index.has_value())
            return 1;   // данные были записаны в контейнер m_extraFields

        return SWorkModel::setData(srm_index.value(), data);
    };
    void initSingleRowModelField(const int index, const QVariant &data) override {
        auto srm_index = setExtraData(index, data);
        if(!srm_index.has_value())
            return;     // данные были записаны в контейнер m_extraFields
        initFieldWithPrevLoaded(srm_index.value(), data);
    };
};

/**************************************************************************************/
class StoreSaleModel : public SSaleTableModel {
    Q_OBJECT
    friend class TClassTest;
public:
    enum OpType {Reserve = 0, Sale, Unsale, FreeReserved};
    enum State {New = 0, Sold = 1, Reserved = 2, Cancelled = 3};
    static const int SaleMode = 0x04;
    StoreSaleModel(QObject *parent = nullptr);
    bool loadTable(const int doc_id) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void removeRow(const int row) override;
//    bool removeItems() override {return 1;};
//    bool removeWorks() override {return 1;};
    void markRowRemove(const int row) override;
    bool backOutItems(OpType);
    void markAllItemsToRemove(OpType type);
    void prepareUnsaleItems(const QString &reason = QString());
    bool unsaleItems(const QString &reason = QString());
    void setExtraUnsaleReason(const QString&);
    void prepareFreeItems();
    bool freeItems();
    bool endModelChange() override {return 1;};
    bool commit(int operation) override;
    virtual Qt::ItemFlags flags(const QModelIndex&) const override;
    int pixmapFlags(const QModelIndex&) const override;
private:
    QString m_extraUnsaleReason;
    bool isModelEditable() const override;
    bool checkBeforeItemAdd(const int id, const int count) override;
    bool isRowMarkedRemove(const int) const override;
    int itemInsertionRow() override;
    int saleMode() const override {return SaleMode;}
public slots:
#if defined QT_DEBUG || defined S_TEST
    void dbgAddRandomItem();
#endif
};

class WorkshopSaleModel : public SSaleTableModel {
    Q_OBJECT
    friend class TClassTest;
public:
    enum OpType {Reserve = 0, Link, Sale, Unsale, Unlink, Update};
    enum State {RW = 0, RO = 1, Adm = 2};
    enum Type {Regular = 0, Cartridge};
    static const int SaleMode = 0x00;
    WorkshopSaleModel(QObject *parent = nullptr);
    ~WorkshopSaleModel();
    void initDemo() override;
    bool loadTable(const int doc_id) override;
    bool addItemFromBasket(const int id, const int qty = 0);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void removeRow(const int row) override;
    void markRowRemove(const int row) override;
    using SStandardItemModel::removeRows;
    bool removeRows();
    bool isDirty() override;
    bool commit(int operation) override;
    bool commit();
    bool commitStandalone();
    bool endModelChange() override;
    virtual Qt::ItemFlags flags(const QModelIndex&) const override;
    int editStrategy();
    void setEditStrategy(const int);
    void setRepairType(bool type);
    void setCartridgeCardModel(SCartridgeCardModel *cartridgeCardModel);
#if defined QT_DEBUG || defined S_TEST
    void dbgAddRandomItem();
    void dbgAddRandomItemBasket();
#endif
private:
    bool m_repairType = Type::Regular;
    int m_editStrategy = EditStrategy::OnFieldChange;
    SCartridgeCardModel *m_cartridgeCardModel = nullptr;
    bool isModelEditable() const override;
    bool checkBeforeItemAdd(const int id, const int count) override;
    bool initSingleRowModel(const QSqlRecord &record, std::shared_ptr<SSingleRowJModel> model);
    void itemAddHook(const int row) override;
    bool isRowMarkedRemove(const int) const override;
    int itemInsertionRow() override;
    int saleMode() const override {return SaleMode;}
};

/************ авто-регистрация методов создания моделей ******************************/
static TypeFabricRegister<SStoreSaleItemModelExt> StoreSaleItemModelExt(StoreSaleModel::SaleMode | SSaleTableModel::RecordType::Item);
static TypeFabricRegister<SRepairSaleItemModelExt> RepairSaleItemModelExt(WorkshopSaleModel::SaleMode | SSaleTableModel::RecordType::Item);
static TypeFabricRegister<SWorkModelExt> SWorkModelExt(WorkshopSaleModel::SaleMode | SSaleTableModel::RecordType::Work);

#endif // SSALETABLEMODEL_H
