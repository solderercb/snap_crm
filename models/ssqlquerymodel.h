#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
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
    void setDisplayRoleColumn(const QString&);
    int findIndex(const QString&);
private:
    int m_displayRoleColumn = -1;
    void initDisplayRoleColumn();

signals:

};

#endif // SSQLQUERYMODEL_H
