#include "scartridgecardmodel.h"

SCartridgeCardModel::SCartridgeCardModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "cartridge_cards";
    i_obligatoryFields << "name" << "maker" << "created" << "user" << "notes";
    i_idColumnName = "id";
}

SCartridgeCardModel::~SCartridgeCardModel()
{
    while(!materials.isEmpty())
        materials.remove(materials.lastKey());
}

int SCartridgeCardModel::id()
{
    return i_id;
}

void SCartridgeCardModel::load(const int id)
{
    if(!id)
        return;

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("SELECT `name`, `maker`, `full_weight`, `toner_weight`, `resource`, `created`, `user`, `notes`, `photo`, `color`, `archive` FROM `cartridge_cards` WHERE `id` = %1;").arg(id));
    if(!record->first())
        return;

    i_id = id;

    m_name = record->value("name").toString();
    m_maker = record->value("maker").toInt();
    m_fullWeight = record->value("full_weight").toDouble();
    m_tonerWeight = record->value("toner_weight").toDouble();
    m_resource = record->value("resource").toInt();
    i_createdUtc = record->value("created").toDateTime();
    m_user = record->value("user").toInt();
    m_notes = record->value("notes").toString();
    m_photo = record->value("photo").toInt();
    m_color = record->value("color").toInt();
    m_archive = record->value("archive").toBool();
    initMaterials();

    delete record;
}

void SCartridgeCardModel::initMaterials()
{
    QSqlQuery query(QSqlDatabase::database("connMain"));
    SCartridgeMaterialModel *material;

    query.exec(QString("SELECT `id` FROM `materials` WHERE `card_id` = %1;").arg(i_id));
    while(query.next())
    {
        material = new SCartridgeMaterialModel();
        material->load(query.value(0).toInt());
        materials.insert(material->type(), material);
    }
}

QString SCartridgeCardModel::name()
{
    return m_name;
}

void SCartridgeCardModel::setName(const QString& name)
{
    i_valuesMap.insert("name", name);
}

int SCartridgeCardModel::maker()
{
    return m_maker;
}

void SCartridgeCardModel::setMaker(const int maker)
{
    i_valuesMap.insert("maker", maker);
}

double SCartridgeCardModel::fullWeight()
{
    return m_fullWeight;
}

void SCartridgeCardModel::setFullWeight(const double full_weight)
{
    i_valuesMap.insert("full_weight", full_weight);
}

double SCartridgeCardModel::tonerWeight()
{
    return m_tonerWeight;
}

void SCartridgeCardModel::setTonerWeight(const double toner_weight)
{
    i_valuesMap.insert("toner_weight", toner_weight);
}

int SCartridgeCardModel::resource()
{
    return m_resource;
}

void SCartridgeCardModel::setResource(const int resource)
{
    i_valuesMap.insert("resource", resource);
}

void SCartridgeCardModel::setCreated(const int created)
{
    i_valuesMap.insert("created", created);
}

int SCartridgeCardModel::user()
{
    return m_user;
}

void SCartridgeCardModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

QString SCartridgeCardModel::notes()
{
    return m_notes;
}

void SCartridgeCardModel::setNotes(const QString& notes)
{
    i_valuesMap.insert("notes", notes);
}

int SCartridgeCardModel::photo()
{
    return m_photo;
}

void SCartridgeCardModel::setPhoto(const int photo)
{
    i_valuesMap.insert("photo", photo);
}

int SCartridgeCardModel::color()
{
    return m_color;
}

void SCartridgeCardModel::setColor(const int color)
{
    i_valuesMap.insert("color", color);
}

bool SCartridgeCardModel::archive()
{
    return m_archive;
}

void SCartridgeCardModel::setArchive(const bool archive)
{
    i_valuesMap.insert("archive", archive);
}

SCartridgeMaterialModel *SCartridgeCardModel::material(const int type)
{
    return materials.value(type, 0);
}

