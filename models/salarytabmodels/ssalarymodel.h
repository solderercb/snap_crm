#ifndef SSALARYMODEL_H
#define SSALARYMODEL_H

#include <SSingleRowModel>

#include "../ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(user_id, employee, int, 0)                                  \
    TABLE_FIELD(summ, amount, double, 0)                                    \
    TABLE_FIELD(balance, balance, double, 0)                                \
    TABLE_FIELD(balance_record, balanceRecord, int, 0)                      \
    TABLE_FIELD(payment_date, paymentDate, QDateTime, 0)                    \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(period_from, periodFrom, QDateTime, 0)                      \
    TABLE_FIELD(period_to, periodTo, QDateTime, 0)                          \
    TABLE_FIELD(from_user, user, int, 0)                                    \
    TABLE_FIELD(type, type, bool, 0)                                        \

class SSalaryModel : public SSingleRowModel
{
    Q_OBJECT
public:
    enum Type{Salary = 0, Subsistence = 1};
    explicit SSalaryModel(QObject *parent = nullptr);
#include "../ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load(const int id);
protected:
    int indexOfCreated() override;
private:
};

inline int SSalaryModel::indexOfCreated()
{
    return C_paymentDate;
}

#endif // SSALARYMODEL_H
