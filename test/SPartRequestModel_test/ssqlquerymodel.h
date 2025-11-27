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
    QString getDisplayRole(int id, QString = "id"){if(id == 1) return "test1"; else if(id == 2) return "test 2"; else return "test3";};
    QString getDisplayRole(int id, int){return getDisplayRole(id);};
    int databaseIDByRow(int id, QString idField = "id"){Q_UNUSED(idField) if(id == 0) return 0; else if(id == 1) return 1; else return 2;};
    int rowByDatabaseID(int id, int){return rowByDatabaseID(id);};
    int rowByDatabaseID(int id, QString field = "id"){Q_UNUSED(field) return databaseIDByRow(id);};
    virtual void clear(){};
    QVariant value(int id, QString, QString){if(id == 1) return "<value 1>"; return "<value 2>";};
};

#endif // SSQLQUERYMODEL_H
