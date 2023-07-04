#include "scartridgematerialmodel.h"

SCartridgeMaterialModel::SCartridgeMaterialModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "materials";
    i_obligatoryFields << "type" << "name" << "card_id";
    i_idColumnName = "id";
}

int SCartridgeMaterialModel::id()
{
    return i_id;
}

void SCartridgeMaterialModel::load(const int id)
{
    if(!id)
        return;

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("SELECT `name`, `type`, `count`, `articul`, `card_id`, `price`, `works_price` FROM `materials` WHERE `id` = %1;`").arg(id));
    if(!record->first())
        return;

    i_id = id;

    m_type = record->value("type").toInt();
    m_count = record->value("count").toInt();
    m_articul = record->value("articul").toInt();
    m_name = record->value("name").toString();
    m_cardId = record->value("card_id").toInt();
    m_price = record->value("price").toDouble();
    m_worksPrice = record->value("works_price").toDouble();

    delete record;
}

int SCartridgeMaterialModel::type()
{
    return m_type;
}

void SCartridgeMaterialModel::setType(const int type)
{
    i_valuesMap.insert("type", type);
}

int SCartridgeMaterialModel::count()
{
    return m_count;
}

void SCartridgeMaterialModel::setCount(const int count)
{
    i_valuesMap.insert("count", count);
}

int SCartridgeMaterialModel::articul()
{
    return m_articul;
}

void SCartridgeMaterialModel::setArticul(const int articul)
{
    i_valuesMap.insert("articul", articul);
}

QString SCartridgeMaterialModel::name()
{
    return m_name;
}

void SCartridgeMaterialModel::setName(const QString& name)
{
    i_valuesMap.insert("name", name);
}

int SCartridgeMaterialModel::cardId()
{
    return m_cardId;
}

void SCartridgeMaterialModel::setCardId(const int card_id)
{
    i_valuesMap.insert("card_id", card_id);
}

double SCartridgeMaterialModel::price()
{
    return m_price;
}

void SCartridgeMaterialModel::setPrice(const double price)
{
    i_valuesMap.insert("price", price);
}

double SCartridgeMaterialModel::worksPrice()
{
    return m_worksPrice;
}

void SCartridgeMaterialModel::setWorksPrice(const double works_price)
{
    i_valuesMap.insert("works_price", works_price);
}

