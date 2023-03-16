#ifndef STABLEVIEWREPAIRSITEMDELEGATES_H
#define STABLEVIEWREPAIRSITEMDELEGATES_H

#include "stableviewbaseitemdelegates.h"
#include <QObject>
#include <QPainter>

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
};

#endif // STABLEVIEWREPAIRSITEMDELEGATES_H
