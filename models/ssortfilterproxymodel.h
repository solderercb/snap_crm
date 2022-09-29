#ifndef SSORTFILTERPROXYMODEL_H
#define SSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "sabstractitemmodel.h"
#include <QObject>

class SSortFilterProxyModel : public QSortFilterProxyModel, public SAbstractItemModel
{
    Q_OBJECT
public:
    explicit SSortFilterProxyModel(QObject *parent = nullptr);
    ~SSortFilterProxyModel();
};

#endif // SSORTFILTERPROXYMODEL_H
