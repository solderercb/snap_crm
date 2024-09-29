#ifndef SPARTSUPPLIER_H
#define SPARTSUPPLIER_H

#include "models/scomrecord.h"
#include <QObject>

class SPartSupplier : public SComRecord
{
    Q_OBJECT
public:
    explicit SPartSupplier(QObject *parent = nullptr);
    int id();
    void load(const int &id);
    int requestId();
    void setRequestId(const int &id);
    int supplierId();
    void setSupplierId(const int &id);
    QString supplierUrl();
    void setSupplierUrl(const QString &url);
    QString itemUrl();
    void setItemUrl(const QString &url);
    int moq();
    void setMoq(const int &moq);
    double price();
    void setPrice(const double &price);
    QString notes();
    void setNotes(const QString &notes);
protected:
    int m_requestId;
    int m_supplierId;
    QString m_supplierUrl;
    QString m_itemUrl;
    int m_moq;
    double m_price;
    QString m_notes;
};

#endif // SPARTSUPPLIER_H
