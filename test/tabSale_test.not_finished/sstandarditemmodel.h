#ifndef SSTANDARDITEMMODEL_H
#define SSTANDARDITEMMODEL_H

#include <QAbstractItemModel>
#include <QString>

class SStandardItemModel : public QAbstractItemModel
{
public:
    explicit SStandardItemModel(){};
    ~SStandardItemModel(){};
    QString getDisplayRole(int id, int idColumn){
        return "test1";
    };
    QString getDisplayRole(int id, QString idField = "id"){
      if(id == 1)
          return "test1";
      else
          return "test2";
    };

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

QModelIndex SStandardItemModel::index(int row, int column, const QModelIndex &parent = QModelIndex()) const
{
    return QModelIndex();
}

QModelIndex SStandardItemModel::parent(const QModelIndex &child = QModelIndex()) const
{
    return QModelIndex();
}

int SStandardItemModel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
    return 1;
}

int SStandardItemModel::columnCount(const QModelIndex &parent = QModelIndex()) const
{
    return 2;
}

QVariant SStandardItemModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

#endif // SSTANDARDITEMMODEL_H
