#ifndef SWORKSHOPISSUEDMODEL_H
#define SWORKSHOPISSUEDMODEL_H

#include "scomrecord.h"
#include <QObject>

class SWorkshopIssuedModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SWorkshopIssuedModel(QObject *parent = nullptr);
    int id();
    int repair();
    void setRepair(const int);
    int employee();
    void setEmployee(const int);
    void load(const int id);
    bool commit();
private:
    int m_repair;
    int m_employeeId;
};

#endif // SWORKSHOPISSUEDMODEL_H
