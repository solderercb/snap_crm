#ifndef SWORKMODEL_H
#define SWORKMODEL_H

#include "scomrecord.h"
#include <QObject>
#include "models/sstoreitemmodel.h"

class SWorkModel : public SComRecord
{
    Q_OBJECT
public:
    enum Type{Regular = 0, CartridgeRefill, CartridgeChipReplace, CartridgeDrumReplace, CartridgeBladeReplace, CartridgeReplaceOfWorn,
              CartridgeRefillPlus, CartridgeMagRollerReplace,  CartridgeProphilaxy, CartridgePrimRollerReplace,
              CartridgeMarkerReplace, CartridgeScraperReplace, CartridgeBushingsReplace};
    Q_ENUM(Type)
    explicit SWorkModel(QObject *parent = nullptr);
    explicit SWorkModel(const QList<QStandardItem *> &record, QObject *parent = nullptr);
    int id();
    void load(const int);
    int user();
    void setUser(const int id, const QVariant oldValue = QVariant());
    int repair();
    void setRepair(const int);
    int documentId();
    void setDocumentId(const int);
    QString name();
    void setName(const QString name, const QVariant oldValue = QVariant());
    double price();
    void setPrice(const double price, const QVariant oldValue = QVariant());
    int count();
    void setCount(const int count, const QVariant oldValue = QVariant());
    int warranty();
    void setWarranty(const int warranty, const QVariant oldValue = QVariant());
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
    bool remove();
    void setQueryField(const int fieldNum, const QVariant value, const QVariant oldValue = QVariant()) override;
    bool commit();
    double salarySumm() const;
    void setSalarySumm(double salarySumm);

private:
    int m_user;
    int m_repair;
    int m_documentId;
    QString m_name;
    double m_price;
    int m_count;
    int m_warranty;
    int m_priceId;
    bool m_isPay;
    int m_type;
    int m_payRepair;
    int m_payRepair_quick;
    double m_salarySumm;
};

#endif // SWORKMODEL_H
