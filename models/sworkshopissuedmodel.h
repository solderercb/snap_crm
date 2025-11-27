#ifndef SWORKSHOPISSUEDMODEL_H
#define SWORKSHOPISSUEDMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(repair_id, repair, int, 0)                               \
    TABLE_FIELD(employee_id, employee, int, 0)                           \
    TABLE_FIELD(created_at, created, QDateTime, 0)

class SWorkshopIssuedModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SWorkshopIssuedModel(QObject *parent = nullptr);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    bool commit();
};

#endif // SWORKSHOPISSUEDMODEL_H
