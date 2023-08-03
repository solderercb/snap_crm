#ifndef SSQLFETCHINGMODEL_H
#define SSQLFETCHINGMODEL_H

#include <QStandardItemModel>
#include <QSqlQuery>
#include <QObject>
#include <QDebug>

class SSqlFetchingModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit SSqlFetchingModel(QObject *parent = nullptr);
    ~SSqlFetchingModel();
    int fetchSize() const;
    void setFetchSize(int fetchSize);
    void clear();
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void fetchMore(const QModelIndex &parent = QModelIndex());
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QModelIndex parent(const QModelIndex &child) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
private:
    QString m_query;
    QSqlDatabase m_db;
    QSqlQuery *m_proxyQuery = nullptr;
    int m_rowCount = 0;
    int m_columnCount = 0;
    int m_wholeRowCount = 0;
    int m_fetchSize = 30;
    void queryRowCount();
    void copyRowsFromSource();
public:
};

#endif // SSQLFETCHINGMODEL_H
