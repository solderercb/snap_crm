#ifndef STABLEREPAIRSMODEL_H
#define STABLEREPAIRSMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QColor>
#include <QStandardItemModel>
#include <QStandardItem>
#include "models/stablebasemodel.h"
#include "global.h"

class STableRepairsModel : public STableBaseModel
{
    Q_OBJECT

public:
    explicit STableRepairsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
private:
    QVector<uint32_t> status_ids;
    QVector<uint32_t> status_ids_tmp;
    QVector<QStandardItem*> status;
    QVector<QVariant> colors;
    QColor *color;

};

#endif // STABLEREPAIRSMODEL_H
