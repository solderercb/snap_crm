#ifndef SOFFICEMODEL_H
#define SOFFICEMODEL_H

#include <QObject>
#include "scomrecord.h"

class SOfficeModel : public SComRecord
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(int state READ state)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QString phone READ phone)
    Q_PROPERTY(QString phone2 READ phone2)
    Q_PROPERTY(QByteArray logo READ logo)
    Q_PROPERTY(int admin READ admin)
    Q_PROPERTY(QString created READ created)
    Q_PROPERTY(int defaultCompany READ defaultCompany)
    Q_PROPERTY(bool cardPayment READ cardPayment)
public:
    explicit SOfficeModel(QObject *parent = nullptr);
    ~SOfficeModel();
    static SOfficeModel* current(QObject *parent = nullptr);
    void load(const int &id);
    const int& id();
    const int& state();
    void setState(const int &state);
    const QString& name();
    void setName(const QString &name);
    const QString& address();
    void setAddress(const QString &address);
    const QString& phone();
    void setPhone(const QString &phone);
    const QString& phone2();
    void setPhone2(const QString &phone2);
    const QByteArray& logo();
    void setLogo(const QByteArray &logo);
    const int& admin();
    void setAdmin(const int &id);
    int defaultCompany();
    int defaultCompanyIndex();
    void setDefaultCompany(const int &id);
    const bool& cardPayment();
    void setCardPayment(const bool &state);
    const bool& useBoxes();
    void setUseBoxes(const bool &state);
    const bool& paintRepairs();
    void setPaintRepairs(const bool &state);
    const bool& warrantySN();
    void setWarrantySN(const bool &state);
private:
    static SOfficeModel* p_current;
    int m_state;
    QString m_name;
    QString m_address;
    QString m_phone;
    QString m_phone2;
    QByteArray m_logo;
    int m_admin;
    int m_defaultCompany;
    bool m_cardPayment;
    bool m_useBoxes;
    bool m_paintRepairs;
    bool m_warrantySN;
};

#endif // SOFFICEMODEL_H
