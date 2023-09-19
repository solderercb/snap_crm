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
    QString getDisplayRole(int id, int idColumn);
    QString getDisplayRole(int id, QString idField = "id");
    QVariant value(int id, int idColumn, int dataColumn);
    QVariant value(int id, QString idField = "id", QString dataField = "name");
    int rowByDatabaseID(int id, int column);
    int rowByDatabaseID(int id, QString field = "id");
    int databaseIDByRow(int row, int column);
    int databaseIDByRow(int row, QString field = "id");
    template<typename T> int valueColumnToRow(T value, int column);
    template<typename T> T rowColumnToValue(int row, int column);
    void setDisplayRoleColumn(const QString&);
    int findIndex(const QString&);
    void initDisplayRoleColumn();
protected:
    int m_displayRoleColumn = -1;
    QAbstractItemModel *abstractItemModel;
};

/*  Возвращает номер строки, соответствующий value из столбца column
 *  Если значение не найдено, возвращает -1
*/
template<typename T>
int SAbstractItemModel::valueColumnToRow(T value, int column)
{
    if(abstractItemModel->rowCount() == 0)
        return -1;

    for(int i=0; i<abstractItemModel->rowCount(); i++)
    {
        if(abstractItemModel->index(i, column).data().value<T>() == value)
        {
            return i;
        }
    }

    return -1;
}

/* Возвращает значение типа T, соответствующее строке row и столбцу column.
*/
template<typename T>
T SAbstractItemModel::rowColumnToValue(int row, int column)
{
    return abstractItemModel->index(row, column).data().value<T>();
}

#endif // SABSTRACTITEMMODEL_H
