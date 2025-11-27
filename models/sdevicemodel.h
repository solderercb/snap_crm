#ifndef SDEVICEMODEL_H
#define SDEVICEMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(position, position, int, 0)                                 \
    TABLE_FIELD(enable, enable, bool, 0)                                    \
    TABLE_FIELD(refill, refill, bool, 0)                                    \
    TABLE_FIELD(fault_list, faultList, QString, 0)                          \
    TABLE_FIELD(look_list, lookList, QString, 0)                            \
    TABLE_FIELD(complect_list, complectList, QString, 0)                    \
    TABLE_FIELD(company_list, companyList, QString, 0)

class SDeviceModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SDeviceModel(QObject *parent = nullptr);
    ~SDeviceModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
private:
};

#endif // SDEVICEMODEL_H
