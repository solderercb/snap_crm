#ifndef SSQLFETCHINGMODEL_H
#define SSQLFETCHINGMODEL_H

#include <QObject>
#include <QModelIndex>
#include <QVariant>
#include <QMetaObject>

/* Заглушка для теста */
class SSqlFetchingModel
{
public:
    explicit SSqlFetchingModel(QObject *parent = nullptr);
    QModelIndex index(int, int, const QModelIndex& = QModelIndex()) const {return QModelIndex();};
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const {return QVariant();};
    int rowCount(const QModelIndex& = QModelIndex()) const {return 0;};
    QMetaObject* metaObject(){return nullptr;};
};

#endif // SSQLFETCHINGMODEL_H
