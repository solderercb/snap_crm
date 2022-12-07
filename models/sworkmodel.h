#ifndef SWORKMODEL_H
#define SWORKMODEL_H

#include "scomrecord.h"
#include <QObject>

class SWorkModel : public SComRecord
{
    Q_OBJECT
public:
    enum Columns {ColId = 0, ColUser, ColRepairId, ColDocumentId, ColName, ColPrice, ColCount, ColWarranty, ColPriceId, ColIsPay, ColAdded, ColType, ColPayRepair, ColPayRepairQuick};
    explicit SWorkModel(QObject *parent = nullptr);
    explicit SWorkModel(const QList<QStandardItem *> &record, QObject *parent = nullptr);
    int id();
    void load(const int);
    int user();
    void setUser(const int);
    int repair();
    void setRepair(const int);
    int documentId();
    void setDocumentId(const int);
    QString name();
    void setName(const QString);
    float price();
    void setPrice(const float);
    int count();
    void setCount(const int);
    int warranty();
    void setWarranty(const int);
    int priceId();
    void setPriceId(const int);
    bool isPay();
    void setIsPay(const bool);
    void setCreated(const QDateTime);
    int type();
    void setType(const int);
    int payRepair();
    void setPayRepair(const int);
    int payRepairQuick();
    void setPayRepairQuick(const int);
    bool update();
    bool remove();
    bool commit();
private:
    int m_user;
    int m_repair;
    int m_documentId;
    QString m_name;
    float m_price;
    int m_count;
    int m_warranty;
    int m_priceId;
    bool m_isPay;
    int m_type;
    int m_payRepair;
    int m_payRepair_quick;

};

#endif // SWORKMODEL_H
