#ifndef SPAYMENTTYPESMODEL_H
#define SPAYMENTTYPESMODEL_H

#include "com_sql_queries.h"
#include "sstandarditemmodel.h"
#include <QObject>
#include <QSqlQueryModel>

class SPaymentTypesModel : public SStandardItemModel
{
    Q_OBJECT
public:
    enum Type {PKO = 0, RKO = 1};
    explicit SPaymentTypesModel(const int type, QObject *parent = nullptr);
    void loadExtraTypes(const int type, const bool loadArchive);
    QVariant dataByDatabaseId(int id, int role = Qt::DisplayRole);
    QString reasonByDatabaseId(int id);
    int clientByDatabaseId(int id);
    int paymentSystemByDatabaseId(int id);
    float defaultAmountByDatabaseId(int id);
};

#endif // SPAYMENTTYPESMODEL_H
