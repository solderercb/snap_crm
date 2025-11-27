#include "sworkpricemodel.h"
#include <QSqlQuery>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SStandardItemModel>
#include <SLogRecordModel>

SWorkPriceModel::SWorkPriceModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "workshop_price";
    i_obligatoryFields << "name" << "category";
    setPrimaryKeyIndex(0);

    i_logRecord->set_type(SLogRecordModel::Part);
    initSqlQuery();
}

SWorkPriceModel::~SWorkPriceModel()
{
}

SStandardItemModel *SWorkPriceModel::priceOptionsList()
{
    SStandardItemModel *list;
    QMetaEnum types = SWorkPriceModel::staticMetaObject.enumerator(SWorkPriceModel::staticMetaObject.indexOfEnumerator("PriceOption"));
    QVector<QString> priceColDBFieldsList = {"`price1`", "`price2`", "0"};
    QList<QStandardItem*> dbFields;

    list = SStandardItemModel::modelFromEnum(types, tr);
    for (int i=0; i<priceColDBFieldsList.size(); i++)
    {
        dbFields << new QStandardItem(priceColDBFieldsList.at(i));
    }
    list->appendColumn(dbFields);
    list->setObjectName("workPriceColModel");
    list->setHorizontalHeaderLabels({"name", "id", "dbColumn"});

    return list;
}

void SWorkPriceModel::translateNames()
{
}

