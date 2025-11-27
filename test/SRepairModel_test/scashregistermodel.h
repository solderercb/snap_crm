#ifndef SCASHREGISTERMODEL_H
#define SCASHREGISTERMODEL_H

#include <QObject>

class SCashRegisterModel
{
public:
    enum PaymentType{ExpGoods = 9, RecptPrepayRepair = 12, RecptGoods = 14};
    explicit SCashRegisterModel(QObject *parent = nullptr);
    int id(){return 92650;};
    void setId(int);
    void setCompany(int);
    void setDocumentId(int);
    void setSystemId(int);
    void setClient(int);
    void unsetClient();
    void setOperationType(int);
    void setReason(const QString&);
    int lastInsertId(){return 123456;};
    bool commit(double){return 1;};
};

#endif // SCASHREGISTERMODEL_H
