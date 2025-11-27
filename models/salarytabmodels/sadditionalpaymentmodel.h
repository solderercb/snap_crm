#ifndef SADDITIONALPAYMENTMODEL_H
#define SADDITIONALPAYMENTMODEL_H

#include <SSingleRowJModel>

#include "../ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(payment_date, paymentDate, QDateTime, 0)                    \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(to_user, employee, int, 0)                                  \
    TABLE_FIELD(price, summ, double, 0)

class SAdditionalPaymentModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SAdditionalPaymentModel(QObject *parent = nullptr);
#include "../ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    bool commit();
};

#endif // SADDITIONALPAYMENTMODEL_H
