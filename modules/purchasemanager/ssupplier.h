#ifndef SPARTSUPPLIER_H
#define SPARTSUPPLIER_H

#include <SSingleRowJModel>

#include "../../models/ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(request_id, requestId, int, 0)                              \
    TABLE_FIELD(supplier_id, supplierId, int, 0)                            \
    TABLE_FIELD(supplier_url, supplierUrl, QString, 0)                      \
    TABLE_FIELD(item_url, itemUrl, QString, 0)                              \
    TABLE_FIELD(moq, moq, int, 0)                                           \
    TABLE_FIELD(price, price, double, 0)                                    \
    TABLE_FIELD(select, select, int, 0)                                     \
    TABLE_FIELD(notes, notes, QString, 0)

class SPartSupplier : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SPartSupplier(QObject *parent = nullptr);
#include "../../models/ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    using SSingleRowJModel::load;
    void load(const int &id);
protected:
};

#endif // SPARTSUPPLIER_H
