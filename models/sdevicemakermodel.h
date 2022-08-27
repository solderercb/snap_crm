#ifndef SDEVICEMAKERMODEL_H
#define SDEVICEMAKERMODEL_H

#include "scomrecord.h"
#include <QObject>

class SDeviceMakerModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SDeviceMakerModel(QObject *parent = nullptr);
    ~SDeviceMakerModel();
    int id();
    void setId(const int);
    QString name();
    void setName(const QString);
private:
    int m_id;
    QString m_name;
};

#endif // SDEVICEMAKERMODEL_H
