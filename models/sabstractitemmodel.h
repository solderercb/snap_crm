#ifndef SABSTRACTITEMMODEL_H
#define SABSTRACTITEMMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QString>

class SAbstractItemModel
{
public:
    explicit SAbstractItemModel(QObject *parent = nullptr);
    ~SAbstractItemModel();
    int getFieldIdByName(const QString &field);
    QString getDisplayRole(int id, int searchColumn);
    QString getDisplayRole(int id, QString searchField = "id");
    int rowByDatabaseID(int id, int column);
    int rowByDatabaseID(int id, QString field = "id");
    int databaseIDByRow(int row, int column);
    int databaseIDByRow(int row, QString field = "id");
    void setDisplayRoleColumn(const QString&);
    int findIndex(const QString&);
    void initDisplayRoleColumn();
protected:
    int m_displayRoleColumn = -1;
    QAbstractItemModel *abstractItemModel;
};

#endif // SABSTRACTITEMMODEL_H
