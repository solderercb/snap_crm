#ifndef SSALEITEMMODEL_H
#define SSALEITEMMODEL_H

#include "global.h"
#include "scomrecord.h"
#include "sclientmodel.h"
#include "widgets/shortlivednotification.h"
#include <QObject>
#include <QSqlRecord>

class SSaleItemModel : public SComRecord
{
    Q_OBJECT
signals:
    void qtyNotEnough();
public:
    enum Operation{ Nop = 0, Sale = 1, Reserve = 2, SaleReserved = 3, SaleRepair = 4, Unsale = 5, Free = 6 };
    enum DBTable {RegularSales = 0, WorkshopSales = 1};
    enum Columns {ColId = 0, ColUID, ColName, ColCount, ColAvail, ColPrice, ColSumm, ColBox, ColSN, ColWarranty, ColRealization, ColRetPercent, ColIsCancellation, ColCancellationReason, ColItemId, ColInPrice, ColDocument, ColDealer, ColCustomer};
    explicit SSaleItemModel(const QList<QStandardItem *> &record, QObject *parent = nullptr);
    ~SSaleItemModel();
    void setId(int);
    void setMode(int);
    void setClient(int);  // установка id клиента
    void unsetClient();
    void setDocumentId(int);
    bool sale();
    bool reserve();
    bool unsale();
    void setUnsaleReason(const QString&);
    bool free();
    bool saleReserve();
    bool saleRepair();
    QSqlRecord* actualStoreQtys();
    bool isProfitable();
    bool integrityStatus();

private:
    bool nIntegrityErr = 1;
    int m_id = 0;
    int m_item_id = 0;
    QString m_item_name;
    int m_dealer = 0;
    int m_customer = 0;
    bool m_isRealization = 0;
    float m_returnPercent = 0;
    float m_inPrice = 0;
    float m_price = 0;
    int m_doc_id = 0;
    int m_mode = 0;   // 0 - обычная продажа, 1 - продажа в ремонте
    Operation m_op_type = Nop;
    int m_count = 0;
    int m_old_count = 0; // только для случаев продажи ранее зарезервированного товара и возврата
    int m_avail = 0;
    QString m_unsaleReason;
    bool commit();
    bool commit(Operation);
    bool checkAvailabilityBefore(QSqlRecord*);
    bool updateStoreItemsTable();
    bool verifyQty(QSqlRecord*, QSqlRecord*);
    void showNotification(const QString&);
    int count();
    bool dealerRoyalty();

};

#endif // SSALEITEMMODEL_H
