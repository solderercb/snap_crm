#ifndef SSALARYREPAIRSMODEL_H
#define SSALARYREPAIRSMODEL_H

#include <QObject>
#include "../sdatabaserecord.h"

class SSalaryRepairsModel : public SDatabaseRecord
{
    Q_OBJECT
public:
    explicit SSalaryRepairsModel(QObject *parent = nullptr);
    int id();
    int repair();
    void setRepair(const int);
    int cartridge();
    void setCartridge(const int);
    int user();
    void setUser(const int);
    double summ();
    void setSumm(const double);
    void setAccountingDate(const QDateTime date);
    bool commit();
private:
    int m_id;
    int m_repair;
    int m_cartridge;
    int m_user;
    double m_summ;
};

#endif // SSALARYREPAIRSMODEL_H
