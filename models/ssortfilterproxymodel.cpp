#include "ssortfilterproxymodel.h"

SSortFilterProxyModel::SSortFilterProxyModel(QObject *parent):
    QSortFilterProxyModel(parent),
    SAbstractItemModel(parent)
{
    abstractItemModel = this;
}

SSortFilterProxyModel::~SSortFilterProxyModel()
{

}
