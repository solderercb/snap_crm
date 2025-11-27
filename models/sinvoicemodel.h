#ifndef SINVOICEMODEL_H
#define SINVOICEMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(num, num, QString, 0)                                       \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(seller, seller, int, 0)                                     \
    TABLE_FIELD(customer, covenantor, int, 0)                               \
    TABLE_FIELD(paid, paid, QDateTime, 0)                                   \
    TABLE_FIELD(tax, tax, double, 0)                                        \
    TABLE_FIELD(summ, summ, double, 0)                                      \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(total, total, double, 0)                                    \
    TABLE_FIELD(state, state, int, 0)                                       \
    TABLE_FIELD(office, office, int, 0)                                     \
    TABLE_FIELD(type, type, int, 0)

class SInvoiceModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    enum State {Declared = 1, Payed = 2, Archived = 3};
    explicit SInvoiceModel(QObject *parent = nullptr);
    ~SInvoiceModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load(int);
    QString covenantorName();
    int clientId();
    bool commit() override;
private:
    QString constructSelectQuery() override;
};

#endif // SINVOICEMODEL_H
