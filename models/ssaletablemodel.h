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
#include "models/ssaleitemmodel.h"

class SSaleTableModel : public QStandardItemModel

{
    Q_OBJECT
public:
    enum Columns {ColId = 0, ColUID, ColName, ColCount, ColAvail, ColPrice, ColSumm, ColBox, ColSN, ColWarranty, ColRealization, ColRetPercent, ColIsCancellation, ColCancellationReason, ColItemId, ColInPrice, ColDocument, ColDealer, ColCustomer};
    enum State {New = 0, Payed, Reserved, Cancelled};
    enum BackOutOpType {FreeReserved = 0, Unsale = 1};
    enum SaleOpType {Reserve = 0, Sale = 1};
    enum DataRoles {QtyBackup = Qt::UserRole, Changed = Qt::UserRole + 1, IsCancellation = Qt::UserRole + 2};
    explicit SSaleTableModel(QObject *parent = nullptr);
    ~SSaleTableModel();
    QVariant value(int, int) const;
    QVariant value(int, QString) const;
    QVariant value(int, QString, int role) const;
    void setModelState(int state) {m_modelState = state;};
    int modelState() {return m_modelState;};
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    bool insertRecord(int, const QSqlRecord&);
    bool appendRecord(const QSqlRecord&);
    bool removeRowHandler(int, int);
    QMap<int, int>* getPendingRemoveList();
    int pendingRemoveItemsCount();
    void setHorizontalHeaderLabels(const QStringList &labels);
    double totalAmount();
    QString totalAmountLocal();
    QMap<QString, int> *fields;
    void setPriceColumn(QSqlQuery*);
    bool load(int);
    void setClient(int);
    void unsetClient();
    void setDocumentId(int);
    void setDocumentState(int state){documentState = state;};
    bool commit();
    bool saleItems(SaleOpType type = Sale);
    bool reserveItems();
    bool unsaleItems();
    bool unsaleItems(const QString&);
    void setUnsaleReason(const QString&);
    bool freeItems();
    bool backOutItems(BackOutOpType);
    int itemsAffected();
    void markAllItemsToRemove(BackOutOpType);
    SSaleItemModel* item(const int);
    bool integrityStatus();

//    void setDocument(int);

signals:
    void dataChanged();
    void amountChanged(float);
    void modelReset();

private:
    QSqlQueryModel *queryData;
    int m_modelState = 0; // 0 - РЅРѕРІР°СЏ Р Рќ (РІСЃС‘ СЂРµРґР°РєС‚РёСЂСѓРµС‚СЃСЏ, СЃС‚СЂРѕРєРё СѓРґР°Р»СЏСЋС‚СЃСЏ); 1 - РїСЂРѕРІРµРґС‘РЅРЅР°СЏ Р Рќ (РЅРµ СЂРµРґР°РєС‚РёСЂСѓРµС‚СЃСЏ, СЃС‚СЂРѕРєРё РїРѕРјРµС‡Р°СЋС‚СЃСЏ СЃРµСЂС‹Рј); 2 - СЂРµР·РµСЂРІ (РІСЃС‘ СЂРµРґР°РєС‚РёСЂСѓРµС‚СЃСЏ, СЃС‚СЂРѕРєРё РїРѕРјРµС‡Р°СЋС‚СЃСЏ СЃРµСЂС‹Рј); 3 - СЂР°СЃРїСЂРѕРІРµРґС‘РЅРЅР°СЏ Р Рќ РёР»Рё СЃРЅСЏС‚С‹Р№ СЂРµР·РµСЂРІ (РЅРµ СЂРµРґР°РєС‚РёСЂСѓРµС‚СЃСЏ, РєРЅРѕРїРѕРє РІ РїРµСЂРІРѕРј СЃС‚РѕР»Р±С†Рµ РЅРµС‚)
    int documentId = 0;
    int documentState = 0;
    int m_client = 0;
    int m_itemsAffected = 0;
    QMap<int, int> *pendingRemoveList;
    QString price_field_name = "`price2`";
    bool amountChangedSignalFilter = 0;
    int countCol = ColCount;
    int priceCol = ColPrice;
    int availCol = ColAvail;
    int summCol = ColSumm;
     QList<QStandardItem *> row(int);
     bool nIntegrityErr = 1;
     QString m_unsaleReason;

    // РЅР°Р·РІР°РЅРёСЏ СЃС‚РѕР»Р±С†РѕРІ РїРѕ-СѓРјРѕР»С‡Р°РЅРёСЋ; РїРѕРґСЂРѕР±РЅРµРµ СЃРј. РІ РєРѕРјРјРµРЅС‚Р°СЂРёРё Рє РјРµС‚РѕРґСѓ SSaleTableModel::setHorizontalHeaderLabels
    QStringList fieldsDep = {"id", "UID", "name", "count", "avail", "price", "summ", "box", "sn", "warranty", "is_realization", "return_percent", "is_cancellation", "cancellation_reason", "item_id", "in_price", "document_id", "dealer", "customer_id"};

private slots:
    void sqlDataChanged();
#if QT_VERSION >= 0x060000
    void dataChanaged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>());
#else
    void dataChanaged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>());
#endif

};

#endif // SSALETABLEMODEL_H
