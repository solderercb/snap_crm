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
    void initDemo();
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
private:
    QVariant clientName(const QModelIndex &index) const;
    QVariant dateTime(const QModelIndex &index) const;
};

#endif // STABLEREPAIRSMODEL_H
