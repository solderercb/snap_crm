#ifndef STABLEVIEWCLIENTSITEMDELEGATES_H
#define STABLEVIEWCLIENTSITEMDELEGATES_H

#include "stableviewbaseitemdelegates.h"
#include <QObject>
#include <QPainter>

class STableViewClientsItemDelegates : public STableViewBaseItemDelegates
{
    Q_OBJECT
public:
    explicit STableViewClientsItemDelegates(QObject *parent = nullptr);
    ~STableViewClientsItemDelegates();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // STABLEVIEWCLIENTSITEMDELEGATES_H
