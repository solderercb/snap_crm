#ifndef SSTANDARDITEMMODEL_H
#define SSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include "sabstractitemmodel.h"

class SStandardItemModel : public QStandardItemModel, public SAbstractItemModel
{
    Q_OBJECT
public:
    explicit SStandardItemModel(QObject *parent = nullptr);
    ~SStandardItemModel();
};

#endif // SSTANDARDITEMMODEL_H
