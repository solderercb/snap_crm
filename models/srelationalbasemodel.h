#ifndef SRELATIONALBASEMODEL_H
#define SRELATIONALBASEMODEL_H

#include <QSqlRelationalTableModel>
#include <STableModelCommonMethods>
#include <QObject>

class SRelationalBaseModel : public QSqlRelationalTableModel, public STableModelsCommonMethods
{
    Q_OBJECT
public:
    explicit SRelationalBaseModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // SRELATIONALBASEMODEL_H
