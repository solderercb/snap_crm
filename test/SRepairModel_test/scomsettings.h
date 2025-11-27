#ifndef SCOMSETTINGS_H
#define SCOMSETTINGS_H

#include <QString>

struct t_repairStatus
{
public:
    explicit t_repairStatus(const QString name) : Name(name){};
    QString Name;
};

struct t_repairStatuses
{
public:
    t_repairStatus operator[](int id){return t_repairStatus(QString("status<%1>").arg(id));};
};

class SComSettings
{
public:
    bool printWorksList() {return 1;};
    QString currencyId() {return "USD";};
    t_repairStatuses repairStatusesVariantCopy(){return m_repairStatuses;};
private:
    t_repairStatuses m_repairStatuses;
};

#endif // SCOMSETTINGS_H
