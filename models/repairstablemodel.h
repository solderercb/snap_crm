#ifndef REPAIRSTABLEMODEL_H
#define REPAIRSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QColor>
#include <QStandardItemModel>
#include <QStandardItem>

class repairsTableModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit repairsTableModel(QObject *parent = nullptr);


//    // Header:
//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

//    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

//    // Basic functionality:
//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

//    // Fetch data dynamically:
//    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

//    bool canFetchMore(const QModelIndex &parent) const override;
//    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

//    // Editable:
//    bool setData(const QModelIndex &index, const QVariant &value,
//                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

//    // Add data:
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

//    // Remove data:
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QVector<uint32_t> status_ids;
    QVector<uint32_t> status_ids_tmp;
    QVector<QStandardItem*> status;
    QVector<QVariant> colors;
    QColor *color;

};

#endif // REPAIRSTABLEMODEL_H
