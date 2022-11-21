#ifndef SREPAIRSTATUSLOG_H
#define SREPAIRSTATUSLOG_H

#include "scomrecord.h"
#include <QObject>

class SRepairStatusLog : public SComRecord
{
    Q_OBJECT
public:
    explicit SRepairStatusLog(const int repair, QObject *parent = nullptr);
    int id();
    void setCreated(const QDateTime);
    int repair();
    void setRepair(const int);
    int status();
    void setStatus(const int);
    int user();
    void setUser(const int);
    int manager();
    void setManager(const int);
    void setManagerIndex(const int);
    int engineer();
    void setEngineer(const int);
    void setEngineerIndex(const int);
    void load(const int id);
    bool commit();
private:
    int m_repair;
    int m_status;
    int m_user;
    int m_manager;
    int m_master;
    void setManager(const QVariant);
    void setEngineer(const QVariant);
};

#endif // SREPAIRSTATUSLOG_H
