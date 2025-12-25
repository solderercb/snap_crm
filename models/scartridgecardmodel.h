#ifndef SCARTRIDGECARDMODEL_H
#define SCARTRIDGECARDMODEL_H

#include <SSingleRowJModel>
#include <SWorkModel>
#include <SCartridgeMaterialModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(maker, vendor, int, 0)                                      \
    TABLE_FIELD(full_weight, fullWeight, double, 0)                         \
    TABLE_FIELD(toner_weight, tonerWeight, double, 0)                       \
    TABLE_FIELD(resource, resource, int, 0)                                 \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(photo, photo, int, 0)                                       \
    TABLE_FIELD(color, color, int, 0)                                       \
    TABLE_FIELD(archive, archive, bool, 0)

class SCartridgeCardModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SCartridgeCardModel(QObject *parent = nullptr);
    ~SCartridgeCardModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    using SSingleRowModel::load;
    void load(const int id);
    void loadError(const int type);
    void initMaterials();
    void removeMaterials();
    bool commit();
    std::shared_ptr<SCartridgeMaterialModel> material(const SCartridgeMaterialModel::Type type);
    std::shared_ptr<SCartridgeMaterialModel> material(const SWorkModel::Type type);
    bool isMaterialSet(const int type);
private:
    QMap<int, std::shared_ptr<SCartridgeMaterialModel>> m_materials;
};

#endif // SCARTRIDGECARDMODEL_H
