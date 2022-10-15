#include "sfieldsmodel.h"

SFieldsModel::SFieldsModel(Type type, QObject *parent) :
    QObject(parent),
    m_isRepair(type)
{
    query = new QSqlQuery(QSqlDatabase::database("connMain"));
}

SFieldsModel::~SFieldsModel()
{
    delete query;
    clear();
}

void SFieldsModel::clear()
{
    SFieldValueModel *f;

    while(!m_fieldsList.isEmpty())
    {
        f = m_fieldsList.last();
        m_fieldsList.removeLast();
        delete f;
    }
}

QList<SFieldValueModel*> SFieldsModel::list()
{
    return m_fieldsList;
}

/*  Инициализация массива виджетов для нового товара или ремонта
 *  isRepair == 1 — инициализация виджетов для класса <id> устройств,
 *  иначе для <id> категории товаров
 */
bool SFieldsModel::init(const int id)
{
    if(id == 0)
        return 0;

    if( !m_fieldsList.isEmpty() )
    {
        clear();
    }

    SFieldValueModel *field;

    query->exec(QUERY_SEL_ADDITIONAL_FIELDS_TYPES((m_isRepair?1:0), id));
    while(query->next())
    {
        field = new SFieldValueModel();
        field->createWidget(query->record());
        add(field);
    }
    return 1;
}

bool SFieldsModel::load(int id)
{
    if(m_repair == 0 && m_item == 0)
        return 0;

    if(m_repair)
        query->exec(QUERY_SEL_REPAIR_ADD_FIELDS(id));
    else
        query->exec(QUERY_SEL_ITEM_ADD_FIELDS(id));

    while(query->next())
    {
        itemHandler(query->record());
    }

    delete query;
    return 1;
}

void SFieldsModel::add(SFieldValueModel *item)
{
    connect(item, SIGNAL(emptied(SFieldValueModel*)), this, SLOT(remove(SFieldValueModel*)));
    m_fieldsList.append(item);
}

void SFieldsModel::remove(SFieldValueModel *item)
{
    m_removeList.append(item);
    int index = m_fieldsList.indexOf(item);
    m_fieldsList.removeAt(index);

}

bool SFieldsModel::isEmpty()
{
    return m_fieldsList.isEmpty();
}

void SFieldsModel::setRepair(const int id)
{
    m_repair = id;
    SFieldValueModel *f;
    foreach(f, m_fieldsList)
    {
        f->setRepairId(id);
    }
}

void SFieldsModel::setItem(const int id)
{
    m_item = id;
    SFieldValueModel *f;
    foreach(f, m_fieldsList)
    {
        f->setItemId(id);
    }
}

bool SFieldsModel::commit()
{
    SFieldValueModel *item;
    foreach(item, m_fieldsList)
    {
        if(!item->commit())
        {
            m_nErr = 0;
            throw 1;
        }
    }

    while( !m_removeList.isEmpty() )
    {
        item = m_removeList.last();
        if(!item->delDBRecord())
        {
            m_nErr = 0;
            throw 1;
        }

        m_removeList.removeLast();
        item->deleteLater();
    }

    return 1;
}

bool SFieldsModel::validate()
{
    bool ret = 1;
    SFieldValueModel *item;
    foreach(item, m_fieldsList)
    {
        ret &= item->validate();
    }
    return ret;
}

void SFieldsModel::resetIds()
{
    SFieldValueModel *item;
    foreach(item, m_fieldsList)
    {
        if(!item->value().isEmpty())
            item->setId(0);
    }
}

SFieldValueModel *SFieldsModel::itemHandler(const QSqlRecord &record)
{
    SFieldValueModel *item = new SFieldValueModel(this);
    item->load(record);
    add(item);
    return item;
}

