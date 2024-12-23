#ifndef STABLEVIEWREPAIRSITEMDELEGATES_H
#define STABLEVIEWREPAIRSITEMDELEGATES_H

#include "stableviewbaseitemdelegates.h"
#include <QObject>
#include <QPainter>
#include "models/stablerepairsmodel.h"

class STableViewRepairsItemDelegates : public STableViewBaseItemDelegates
{
    Q_OBJECT
public:
    explicit STableViewRepairsItemDelegates(QObject *parent = nullptr);
    ~STableViewRepairsItemDelegates();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paintStatusProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintRepairProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintClientInformStatus(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setTableModel(QAbstractItemModel *model) override;
private:
    STableRepairsModel *m_tableModel;
};

#endif // STABLEVIEWREPAIRSITEMDELEGATES_H
