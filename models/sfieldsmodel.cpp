#include "sfieldsmodel.h"

SFieldsModel::SFieldsModel(Type type, QObject *parent) :
    QObject(parent),
    m_type(type)
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

    query->exec(QUERY_SEL_ADDITIONAL_FIELDS_TYPES((m_type?1:0), id));
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
    clear();
    if(m_type == Type::Repair)
        query->exec(QUERY_SEL_REPAIR_ADD_FIELDS(id));
    else
        query->exec(QUERY_SEL_ITEM_ADD_FIELDS(id));

    while(query->next())
    {
        itemHandler(query->record());
    }

    return 1;
}

void SFieldsModel::initDemo()
{
    if( !m_fieldsList.isEmpty() )
    {
        clear();
    }

    SFieldValueModel *field;

    query->exec("SELECT 1 AS 'id', '<extra field 1>' AS 'name', '<value 1>' AS 'value', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '' AS 'comment', 1 AS `printable` \n"\
                "UNION ALL\n"\
                "SELECT 2 AS 'id', '<extra field 2>' AS 'name', '<value 2>' AS 'value', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '' AS 'comment', 1 AS `printable` \n"\
                "UNION ALL\n"\
                "SELECT 3 AS 'id', '<extra field 3>' AS 'name', '<value 3>' AS 'value', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '' AS 'comment', 0 AS `printable` \n"\
                "UNION ALL\n"\
                "SELECT 4 AS 'id', '<extra field 4>' AS 'name', '<value 4>' AS 'value', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '' AS 'comment', 1 AS `printable`;");
    while(query->next())
    {
        field = new SFieldValueModel();
        field->load(query->record());
        add(field);
    }
}

void SFieldsModel::add(SFieldValueModel *item)
{
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

void SFieldsModel::setObjectId(const int id)
{
    SFieldValueModel *f;
    foreach(f, m_fieldsList)
    {
        if(m_type == Type::Repair)
            f->setRepairId(id);
        else
            f->setItemId(id);
    }
}

bool SFieldsModel::commit()
{
    SFieldValueModel *item;
    foreach(item, m_fieldsList)
    {
        item->commit();
    }

    while( !m_removeList.isEmpty() )
    {
        item = m_removeList.last();
        item->delDBRecord();

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

void SFieldsModel::enableEdit()
{
    SFieldValueModel *item;
    foreach(item, m_fieldsList)
    {
        connect(item, SIGNAL(emptied(SFieldValueModel*)), this, SLOT(remove(SFieldValueModel*)));
    }
}

int SFieldsModel::printableFieldsCount()
{
    int count = 0;
    for( SFieldValueModel *i : qAsConst(m_fieldsList) )
        if(i->isPrintable())
            count++;

    return count;
}

QString SFieldsModel::reportFieldName()
{
    if(m_reportFieldIndex == -1)
        return QString();

    return m_fieldsList.at(m_reportFieldIndex)->name();
}

QString SFieldsModel::reportFieldValue()
{
    if(m_reportFieldIndex == -1)
        return QString();

    return m_fieldsList.at(m_reportFieldIndex)->value();
}

SFieldValueModel *SFieldsModel::itemHandler(const QSqlRecord &record)
{
    SFieldValueModel *item = new SFieldValueModel(this);
    item->load(record);
    add(item);
    return item;
}

/* Метод получения данных для отчетов LimeReport
 * Смотри описание метода с таким же названием в классе SComRecord
 */
void SFieldsModel::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackData() | info.dataType = " << info.dataType << "; info.index = " << info.index << "; info.columnName = " << info.columnName;
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = (printableFieldsCount() > 0)?0:1; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = metaObject()->property(info.index + 1).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = metaObject()->property( metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(this); break;
        case LimeReport::CallbackInfo::ColumnCount: data = metaObject()->propertyCount() - 1; break;
        case LimeReport::CallbackInfo::RowCount: data = printableFieldsCount(); break;
    }
}

/* "Навигация" по модели данных
 * Непечатаемые поля будут пропущены.
*/
void SFieldsModel::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackDataChangePos() | type = " << type;

    result = 0;

    if(!printableFieldsCount())
    {
        m_reportFieldIndex = -1;
        return;
    }

    if(type == LimeReport::CallbackInfo::First)
        m_reportFieldIndex = 0;
    else
    {
        if(m_reportFieldIndex+1 >= m_fieldsList.count())
            return;
        while(!m_fieldsList.at(++m_reportFieldIndex)->isPrintable())
        {
            if(m_reportFieldIndex+1 >= m_fieldsList.count())
                return;
        }
    }
    result = 1;
}

#ifdef QT_DEBUG
void SFieldsModel::randomFill()
{
    for(int j=0; j< m_fieldsList.size(); j++)   // автозаполнение обязательных доп. полей
    {
        if ( m_fieldsList[j]->property("fieldRequired").toBool() )
            m_fieldsList[j]->randomFill();
    }
}
#endif
