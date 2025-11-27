#ifndef SSTOREITEMMODEL_H
#define SSTOREITEMMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QDebug>

class SStoreItemModel : public QObject
{
    Q_OBJECT
    friend class TClassTest;
public:
    enum Columns {C_price, C_isRealization, C_returnPercent, C_id, C_inPrice, C_dealer};
    enum SaleMode {Store = 1, Repair = 2};
    enum PriceOption {PriceOptionService = 1, PriceOptionRetail = 2, PriceOptionWholesale = 3, PriceOptionWholesale2 = 4, PriceOptionWholesale3 = 5, PriceOptionWarranty = 255};
    explicit SStoreItemModel(QObject *parent = nullptr){Q_UNUSED(parent)};
    ~SStoreItemModel(){};
    int id(){return m_id;};
    void setPrimaryKey(const QVariant &data){m_id = data.toInt();};
    void setSaleMode(SaleMode ){};
    bool setData(const int, const QVariant&){return 1;};
    // только для теста: записываем в переменную код, чтобы проверить, был ли вызван метод
    bool reserve(const QString &logText){m_logText = logText; m_lastOp = 1; m_commited = 1; return 1;};
    bool saleRepair(const QString &logText){m_logText = logText; m_lastOp = 2; m_commited = 1; return 1;};
    bool unsaleRepair(const QString &logText){m_logText = logText; m_lastOp = 3; m_commited = 1; return 1;};
    bool free(const QString &logText){m_logText = logText; m_lastOp = 4; m_commited = 1; return 1;};
    bool sale(const QString &logText){m_logText = logText; m_lastOp = 5; m_commited = 1; return 1;};
    bool saleStore(const QString &logText){m_logText = logText; m_lastOp = 6; m_commited = 1; return 1;};
    bool unsale(const QString &logText){m_logText = logText; m_lastOp = 7; m_commited = 1; return 1;};
    bool commit(){m_commited = 1; return 1;};
    void commitLogs(){};
    void setCommitedExpense(const int){};
    void setNewExpense(const int){};
    void setSaleEntityId(const int){};
    void initFieldForSale(const int, const QVariant&){};
    void appendLogText(const QString&, const QString&){};
    void initFieldWithPrevLoaded(const int, const QVariant&){};
private:
    int m_id = 0;
    int m_lastOp = -1;
    int m_commited = 0;
    QString m_logText;
};

#endif // SSTOREITEMMODEL_H
