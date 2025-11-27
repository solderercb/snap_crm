#ifndef SDEVICEMAKERMODEL_H
#define SDEVICEMAKERMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(name, name, QString, 0)

class SDeviceMakerModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SDeviceMakerModel(QObject *parent = nullptr);
    ~SDeviceMakerModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
};

#endif // SDEVICEMAKERMODEL_H
