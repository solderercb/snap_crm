#ifndef SINVOICEMODEL_H
#define SINVOICEMODEL_H

#include "scomrecord.h"
#include <QObject>

class SInvoiceModel : public SComRecord
{
    Q_OBJECT
public:
    enum State {Declared = 1, Payed = 2, Archived = 3};
    explicit SInvoiceModel(QObject *parent = nullptr);
    ~SInvoiceModel();
    void load(int);
    int id();
    void setId(int);
    QString num();
    void setNum(const QString);
    int user();
    void setUser(const int);
    int seller();
    void setSeller(const int);
    int covenantorId();
    QString covenantorName();
    void setCovenantor(const int);
    int clientId();
    QDateTime paid();
    void setPaid(const QDateTime);
    double tax();
    void setTax(const double);
    double summ();
    void setSumm(const double);
    QString notes();
    void setNotes(const QString);
    double total();
    void setTotal(const double);
    int state();
    void setState(const int);
    int office();
    void setOffice(const int);
    int type();
    void setType(const int);
    bool commit();
private:
    QString m_num;
    int m_user;
    int m_seller;
    int m_covenantorId;
    QString m_covenantorName;
    int m_clientId;
    QDateTime m_paid;
    double m_tax;
    double m_summ;
    QString m_notes;
    double m_total;
    int m_state;
    int m_office;
    int m_type;
};

#endif // SINVOICEMODEL_H
