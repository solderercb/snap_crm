#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include "sstandarditemmodel.h"

class SSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject *parent = nullptr);
    ~SSqlQueryModel();
    int getFieldIdByName(const QString &field);
    QString getDisplayRole(int id, int searchColumn);
    QString getDisplayRole(int id, QString searchField = "id");
    int rowByDatabaseID(int id, int column);
    int rowByDatabaseID(int id, QString field = "id");
    int databaseIDByRow(int row, int column);
    int databaseIDByRow(int row, QString field = "id");
private:
    int m_displayRoleColumn = -1;

signals:

};

#endif // SSQLQUERYMODEL_H
