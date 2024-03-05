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
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    void fetchMore(const QModelIndex &parent = QModelIndex()) override;
    void fetchMore(const int fetchSize, const QModelIndex &parent = QModelIndex());
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex parent(const QModelIndex &child) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
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
