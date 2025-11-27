#ifndef SSORTFILTERPROXYMODEL_H
#define SSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QRegularExpression>
#include <SAbstractItemModel>

class SSortFilterProxyModel : public SAbstractItemModel
{
public:
    explicit SSortFilterProxyModel(QObject *parent = nullptr);
    void setSourceModel(QAbstractItemModel *sourceModel);
    void setFilterRegularExpression(const QRegularExpression &regularExpression);
    void setFilterKeyColumn(int column);
    int databaseIDByRow(int){return 1;};

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

QModelIndex SSortFilterProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex SSortFilterProxyModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int SSortFilterProxyModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int SSortFilterProxyModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SSortFilterProxyModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

#endif // SSORTFILTERPROXYMODEL_H
