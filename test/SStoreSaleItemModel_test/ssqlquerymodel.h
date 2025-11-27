#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QObject>
#include <QString>

class SSqlQueryModel : public QObject
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject* = nullptr){};
    ~SSqlQueryModel(){};
    QString getDisplayRole(int id, QString = "id"){if(id == 1) return "test1"; else return "test2";};
    virtual void clear(){};
};

#endif // SSQLQUERYMODEL_H
