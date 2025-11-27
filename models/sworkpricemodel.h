#ifndef SWORKPRICEMODEL_H
#define SWORKPRICEMODEL_H

#include <SSingleRowJModel>

class SStandardItemModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после sdatabaserecord.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(category, category, int, 0)                                 \
    TABLE_FIELD(warranty, warranty, int, 0)                                 \
    TABLE_FIELD(price1, price1, double, 0)                                  \
    TABLE_FIELD(price2, price2, double, 0)                                  \
    TABLE_FIELD(master_part, masterPart, int, 0)                            \
    TABLE_FIELD(enable, enable, bool, 0)                                    \
    TABLE_FIELD(notes, notes, QString, 0)
    
class SWorkPriceModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class SRepairSaleItemModel;
public:
    enum PriceOption {Price1 = 1, Price2 = 2, Warranty = 255};
    Q_ENUM(PriceOption)
    explicit SWorkPriceModel(QObject *parent = nullptr);
    ~SWorkPriceModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    static SStandardItemModel *priceOptionsList();
private:
    void translateNames();
};

#endif // SWORKPRICEMODEL_H
