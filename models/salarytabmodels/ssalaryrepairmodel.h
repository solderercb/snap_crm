#ifndef SSALARYREPAIRMODEL_H
#define SSALARYREPAIRMODEL_H

#include <SSingleRowModel>

#include "../ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(accounting_date, accountingDate, QDateTime, 0)              \
    TABLE_FIELD(repair, repair, int, 0)                                     \
    TABLE_FIELD(cartridge, cartridge, int, 0)                               \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(summ, amount, double, 0)

class SSalaryRepairModel : public SSingleRowModel
{
    Q_OBJECT
public:
    explicit SSalaryRepairModel(QObject *parent = nullptr);
#include "../ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
private:
};

#endif // SSALARYREPAIRMODEL_H
