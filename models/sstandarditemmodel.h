#ifndef SSTANDARDITEMMODEL_H
#define SSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include "sabstractitemmodel.h"
#include <QMetaEnum>

class SStandardItemModel : public QStandardItemModel, public SAbstractItemModel
{
    Q_OBJECT
public:
    explicit SStandardItemModel(QObject *parent = nullptr);
    ~SStandardItemModel();
    static SStandardItemModel* modelFromEnum(QMetaEnum &_enum, QString (*contextTr)(const char *, const char *, int) = nullptr);
};

#endif // SSTANDARDITEMMODEL_H
