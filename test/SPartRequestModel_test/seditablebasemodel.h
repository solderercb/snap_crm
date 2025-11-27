#ifndef SEDITABLEBASEMODEL_H
#define SEDITABLEBASEMODEL_H

#include <QSqlError>
#include <QObject>
#include <QSqlDatabase>
#include <QModelIndex>

class SEditableBaseModel : public QObject
{
    Q_OBJECT
Q_SIGNALS:
    void primeInsert(int row, QSqlRecord &record);
public:
    explicit SEditableBaseModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~SEditableBaseModel();
    void setEditStrategy(int){};
    void setTable(const QString&){};
    void select(){};
    int rowCount(){return 2;};
    QModelIndex index(int, int){return QModelIndex();};
    void removeRow(int){};
    void insertRow(int){};
    void setData(QModelIndex, const QVariant){};
    QString filter(){return QString("/**/");};
    void setFilter(const QString&){};
    bool submitAll(){return true;};
    QSqlError lastError(){return QSqlError();};
    void setRecordValue(QSqlRecord&, int, QVariant){};
    bool isDirty(){return 0;};
};

#endif // SEDITABLEBASEMODEL_H
