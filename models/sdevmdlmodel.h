#ifndef SDEVMDLMODEL_H
#define SDEVMDLMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(position, position, int, 0)                                 \
    TABLE_FIELD(maker, maker, int, 0)                                       \
    TABLE_FIELD(device, device, int, 0)

class SDevMdlModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SDevMdlModel(QObject *parent = nullptr);
    ~SDevMdlModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
};

#endif // SDEVMDLMODEL_H
