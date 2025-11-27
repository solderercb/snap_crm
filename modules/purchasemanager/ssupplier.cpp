#include "ssupplier.h"

SPartSupplier::SPartSupplier(QObject *parent)
    : SSingleRowJModel{parent}
{
    mapFields();

    i_obligatoryFields << "request_id" << "supplier_url" << "item_url";
    i_tableName = "parts_requests_suppliers";
    setPrimaryKeyIndex(0);
}

void SPartSupplier::load(const int &id)
{
    setPrimaryKey(id);
    SSingleRowJModel::load();
}
