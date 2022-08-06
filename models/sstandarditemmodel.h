#ifndef SSTANDARDITEMMODEL_H
#define SSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItemModel>

class SStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit SStandardItemModel(QObject *parent = nullptr);
    ~SStandardItemModel();
    int getFieldIdByName(const QString &field);
    QString getDisplayRole(int id, int column);
    QString getDisplayRole(int id, QString field = "id");
    int rowByDatabaseID(int id, int column);
    int rowByDatabaseID(int id, QString field = "id");
    int databaseIDByRow(int row, int colum);
    int databaseIDByRow(int row, QString field = "id");
private:
    int m_displayRoleColumn = -1;

signals:

};

#endif // SSTANDARDITEMMODEL_H
