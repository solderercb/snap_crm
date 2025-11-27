#include "stableviewclientsitemdelegates.h"
#include <QPainter>

STableViewClientsItemDelegates::STableViewClientsItemDelegates(QObject *parent) : STableViewBaseItemDelegates(parent)
{
}

STableViewClientsItemDelegates::~STableViewClientsItemDelegates()
{
}

void STableViewClientsItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    STableViewBaseItemDelegates::paint(painter, option, index);
}
