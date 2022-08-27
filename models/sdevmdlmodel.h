#ifndef SDEVMDLMODEL_H
#define SDEVMDLMODEL_H

#include "scomrecord.h"
#include <QObject>

class SDevMdlModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SDevMdlModel(QObject *parent = nullptr);
    ~SDevMdlModel();
    bool commit();
    int id();
    void setId(const int);
    QString name();
    void setName(const QString);
    int position();
    void setPosition(const int);
    int maker();
    void setMaker(const int);
    int device();
    void setDevice(const int);
private:
    int m_id;
    QString m_name;
    int m_position;
    int m_maker;
    int m_device;
};

#endif // SDEVMDLMODEL_H
