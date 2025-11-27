#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QString>

class SSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject *parent = nullptr){Q_UNUSED(parent)};
    ~SSqlQueryModel(){};
    QString getDisplayRole(int id, int){if(id == 1) return "test1"; else return "test2";};
    QString getDisplayRole(int id, QString = "id"){if(id == 1) return "test1"; else return "test2";};
    int databaseIDByRow(int id, QString idField = "id"){Q_UNUSED(idField) if(id == 0) return 0; else return -1;};
    int rowByDatabaseID(int id, int){return rowByDatabaseID(id);};
    int rowByDatabaseID(int id, QString field = "id"){Q_UNUSED(field) if(id == 0) return 0; else return 1;};
    virtual void clear(){};
    QVariant value(int id, QString, QString){if(id == 1) return 66; return 50;};
};

#endif // SSQLQUERYMODEL_H
