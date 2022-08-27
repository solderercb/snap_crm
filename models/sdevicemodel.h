#ifndef SDEVICEMODEL_H
#define SDEVICEMODEL_H

#include "scomrecord.h"
#include <QObject>

class SDeviceModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SDeviceModel(QObject *parent = nullptr);
    ~SDeviceModel();
    int id();
    void setId(const int);
    QString name();
    void setName(const QString);
    int position();
    void setPosition(const int);
    bool isEnable();
    void setEnabled(const bool);
    bool isRefill();
    void setRefill(const bool);
    QString faultList();
    void setFaultList(const QString);
    QString lookList();
    void setLookList(const QString);
    QString complectList();
    void setComplectList(const QString);
    QString companyList();
    void setCompanyList(const QString);
private:
    int m_id;
    QString m_name;
    int m_position;
    bool m_isEnable;
    bool m_isRefill;
    QString m_faultList;
    QString m_lookList;
    QString m_complectList;
    QString m_companyList;
};

#endif // SDEVICEMODEL_H
