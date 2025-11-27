#include "scartridgecardmodel.h"
#include <SUserSettings>
#include <ProjectGlobals>

SCartridgeCardModel::SCartridgeCardModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "cartridge_cards";
    i_obligatoryFields << "name" << "maker" << "created" << "user" << "notes";

    setPrimaryKeyIndex(0);
}

SCartridgeCardModel::~SCartridgeCardModel()
{
    removeMaterials();
}

void SCartridgeCardModel::load(const int id)
{
    setPrimaryKey(id);

    if(!id)
    {
        loadError(Global::ThrowType::ConditionsError);
        return;
    }
    SSingleRowJModel::load();

    initMaterials();
}

// TODO: аналогичный метод есть в классе SCartridgeRepairModel; нужно их обобщить
void SCartridgeCardModel::loadError(const int type)
{
    QString msg = tr("Не удалось загрузить SCartridgeCardModel; id картриджа: %1").arg(id());
    Global::throwError(type, msg);
}

void SCartridgeCardModel::initMaterials()
{
    QSqlQuery query(QSqlDatabase::database("connMain"));
    SCartridgeMaterialModel *material;

    removeMaterials();
    query.exec(QString("SELECT `id` FROM `materials` WHERE `card_id` = %1;").arg(id()));
    while(query.next())
    {
        material = new SCartridgeMaterialModel();
        material->setInitializerCardId(id());
        material->load(query.value(0).toInt());
        m_materials.insert(material->type(), material);
    }
}

void SCartridgeCardModel::removeMaterials()
{
    SCartridgeMaterialModel *material;

    while(!m_materials.isEmpty())
    {
        material = m_materials.last();
        m_materials.remove(m_materials.lastKey());
        delete material;
    }
}

bool SCartridgeCardModel::commit()
{
    // TODO: запись изменений в журнал
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_created, QDateTime::currentDateTime());
        initMandatoryField(C_user, userDbData->id());
        initMandatoryField(C_notes, QVariant());
    }

    SSingleRowJModel::commit();

    return 1;
}

/*
 * В АСЦ поле Ресурс подразумевает ресурс страниц и его назначение не понятно.
 * В данной программе было решено использовать этот параметр по-другому; это
 * позволит предупредить мастера/приёмщика/клиента о превывшении ресурса
 */

SCartridgeMaterialModel *SCartridgeCardModel::material(const SCartridgeMaterialModel::Type type)
{
    return m_materials.value(type, nullptr);
}

SCartridgeMaterialModel *SCartridgeCardModel::material(const SWorkModel::Type type)
{
    switch(type)
    {
        case SWorkModel::Type::CartridgeRefill:            return material(SCartridgeMaterialModel::Type::Toner);
        case SWorkModel::Type::CartridgeDrumReplace:       return material(SCartridgeMaterialModel::Type::Drum);
        case SWorkModel::Type::CartridgeChipReplace:       return material(SCartridgeMaterialModel::Type::Chip);
        case SWorkModel::Type::CartridgeBladeReplace:      return material(SCartridgeMaterialModel::Type::Blade);
        case SWorkModel::Type::CartridgeReplaceOfWorn:     return material(SCartridgeMaterialModel::Type::Replace);
        case SWorkModel::Type::CartridgeRefillPlus:        return material(SCartridgeMaterialModel::Type::TonerPlus);
        case SWorkModel::Type::CartridgeMagRollerReplace:  return material(SCartridgeMaterialModel::Type::MagRoller);
        case SWorkModel::Type::CartridgeProphilaxy:        return material(SCartridgeMaterialModel::Type::Prophilaxy);
        case SWorkModel::Type::CartridgePrimRollerReplace: return material(SCartridgeMaterialModel::Type::PrimRoller);
        case SWorkModel::Type::CartridgeMarkerReplace:     return material(SCartridgeMaterialModel::Type::Marker);
        case SWorkModel::Type::CartridgeScraperReplace:    return material(SCartridgeMaterialModel::Type::Scraper);
        case SWorkModel::Type::CartridgeBushingsReplace:   return material(SCartridgeMaterialModel::Type::Bushings);
        default: return nullptr;
    }
}

bool SCartridgeCardModel::isMaterialSet(const int type)
{
    return m_materials.contains(type);
}

