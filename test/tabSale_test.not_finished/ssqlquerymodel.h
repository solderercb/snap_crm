#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QString>

class SSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject *parent = nullptr){};
    ~SSqlQueryModel(){};
    QString getDisplayRole(int id, int idField){return getDisplayRole(id);};
    QString getDisplayRole(int id, QString idField = "id"){if(id == 1) return "test1"; else return "test2";};
    int databaseIDByRow(int id, QString idField = "id"){if(id == 0) return 0; else return -1;};
    virtual void clear(){};
};

#endif // SSQLQUERYMODEL_H
