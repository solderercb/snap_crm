#ifndef SSQLFETCHINGMODEL_H
#define SSQLFETCHINGMODEL_H

#include <QStandardItemModel>
#include <QSqlQuery>
#include <QObject>
#include <QDebug>

class SSqlFetchingModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SSqlFetchingModel(QObject *parent = nullptr);
    ~SSqlFetchingModel();
    int fetchSize() const;
    void setFetchSize(int fetchSize);
    void clear() override;
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    bool hasIndex(int row, int column, const QModelIndex &parent = QModelIndex()) const;//    inline QModelIndex createIndex(int row, int column, void *data = nullptr) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    void fetchMore(const QModelIndex &parent = QModelIndex()) override;
    void fetchMore(const int fetchSize, const QModelIndex &parent = QModelIndex());
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
private:
    QString m_query;
    QSqlDatabase m_db;
    int m_rowCount = 0;
    int m_columnCount = 0;
    bool m_atEnd = 1;
    int m_fetchSize = 30;
    QVarLengthArray<int, 10> rowOffsets; // used to calculate indexInQuery of rows
//    QVarLengthArray<int, 56> colOffsets; // used to calculate indexInQuery of columns
    QMap<int, QSqlQuery*> m_sqlQueries;
    QSqlQuery *m_queryPointer;
    int m_queryKey = 0;
    QSqlRecord m_rec;
    void queryRowCount();
    int queryPointer(const int row) const;
    QModelIndex indexInQuery(const QModelIndex &item) const override;
public:
};

#endif // SSQLFETCHINGMODEL_H
