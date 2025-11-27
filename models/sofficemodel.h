#ifndef SOFFICEMODEL_H
#define SOFFICEMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(state, state, int, 1)                                       \
    TABLE_FIELD(name, name, QString, 1)                                     \
    TABLE_FIELD(address, address, QString, 1)                               \
    TABLE_FIELD(phone, phone, QString, 1)                                   \
    TABLE_FIELD(phone2, phone2, QString, 1)                                 \
    TABLE_FIELD(logo, logo, QByteArray, 1)                                  \
    TABLE_FIELD(administrator, admin, int, 1)                               \
    TABLE_FIELD(created, created, QDateTime, 1)                             \
    TABLE_FIELD(default_company, defaultCompany, int, 1)                    \
    TABLE_FIELD(card_payment, cardPayment, bool, 1)                         \
    TABLE_FIELD(use_boxes, useBoxes, bool, 0)                               \
    TABLE_FIELD(paint_repairs, paintRepairs, bool, 0)                       \
    TABLE_FIELD(warranty_sn, warrantySN, bool, 0)

class SOfficeModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SOfficeModel(QObject *parent = nullptr);
    ~SOfficeModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    static SOfficeModel* current(QObject *parent = nullptr);
    void load(const int &id);
    int defaultCompanyIndex();
private:
    static SOfficeModel* p_current;
};

#endif // SOFFICEMODEL_H
