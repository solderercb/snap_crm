#ifndef SABSTRACTITEMMODEL_H
#define SABSTRACTITEMMODEL_H

#include <QAbstractItemModel>

class SAbstractItemModel : public QAbstractItemModel
{
public:
    explicit SAbstractItemModel(QObject *parent = nullptr);
 
};

#endif // SABSTRACTITEMMODEL_H
