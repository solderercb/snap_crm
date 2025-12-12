#include "sfieldsmodel.h"
#include <QHash>
#include <QSqlQuery>
#include <QSqlRecord>
#include <ProjectQueries>
#include <QMetaProperty>
#include <SFieldModel>
#include <FieldFactory>
#include <SLogRecordModel>

SFieldsModel::SFieldsModel(std::unique_ptr<FieldFactory> factory, QObject *parent) :
    QObject(parent)
{
    setFieldFactory(std::move(factory));
}

SFieldsModel::~SFieldsModel()
{
    clear();
}

void SFieldsModel::clear()
{
    m_ownerId = 0;
    while(!m_valueModels.isEmpty())
    {
        m_valueModels.removeLast();
    }
}

/* Возвращает массив указателей на SFieldModel, соответствующих заданной категории
*/
QList<std::shared_ptr<SFieldModel> > SFieldsModel::entitiesList()
{
    QList<std::shared_ptr<SFieldModel>> list;

    if(!m_initDone)
        return list;

    foreach (auto f, m_valueModels)
    {
        list.append(f->m_entityModel);
    }

    return list;

}

/* Возвращает массив указателей на SFieldValueModel, существующих в БД
 * (т. е. загруженных ранее или записанных при жизни этой модели).
 *
*/
QList<std::shared_ptr<SFieldValueModel>> SFieldsModel::valuesList()
{
    QList<std::shared_ptr<SFieldValueModel>> list;

    foreach (auto f, m_valueModels)
    {
        if(f->isPrimaryKeyValid())
            list.append(f);
    }

    return list;
}

void SFieldsModel::setSqlQuery(std::shared_ptr<QSqlQuery> query)
{
    m_query = query;
}

/*  Инициализация массива виджетов
 */
bool SFieldsModel::init()
{
    Q_ASSERT_X(m_category, "Category not set", "call setCategory() before init()");

    if(m_initDone)
        return 1;

    QMap<int, std::shared_ptr<SFieldValueModel>> loadedValues;
    foreach(auto f, m_valueModels)
    {
        if(f->isModelLoaded())
            loadedValues.insert(f->fieldId(), f);
    }

    std::shared_ptr<SFieldModel> entityModel;
    std::shared_ptr<SFieldValueModel> valueModel;

    initSqlQuery();
    m_query->exec(QUERY_SEL_ADDITIONAL_FIELDS_TYPES(m_fieldFactory->type(), m_category));

    while(m_query->next())
    {
        entityModel = std::make_shared<SFieldModel>(m_query->value(0).toInt(), m_database);
        entityModel->createWidget();

        if(loadedValues.contains(entityModel->id()))
        {
            valueModel = loadedValues[entityModel->id()];
            entityModel->setWidgetValue(valueModel->value());
            valueModel->enableEdit(entityModel);
            continue;
        }
        if(loadedValues.isEmpty())
            entityModel->setWidgetDefaultValue(); // дефолтные значения только для абсолютно новой модели

        valueModel = std::make_shared<SFieldValueModel>(entityModel);
        m_fieldFactory->initFieldHandler(valueModel);
        add(valueModel);
    }

    updateValueModelOwner();

    m_initDone = 1;

    return 1;
}

bool SFieldsModel::init(const int id)
{
    if(id == 0)
        return 0;

    if(m_category != id)
        m_initDone = 0;

    m_category = id;
    return init();
}

bool SFieldsModel::load(int id)
{
    clear();

    m_ownerId = id;
    initSqlQuery();
    m_query->exec(m_fieldFactory->query(id));

    while(m_query->next())
    {
        initValueModel(m_query->record());
    }


    return 1;
}

void SFieldsModel::initDemo()
{
    if( !m_valueModels.isEmpty() )
    {
        clear();
    }

    std::shared_ptr<SFieldValueModel> field;

    initSqlQuery();
    m_query->exec("SELECT 1 AS 'id', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '<value 1>' AS 'value', '<extra field 1>' AS 'name', 1 AS 'printable'\n"\
                "UNION ALL\n"\
                "SELECT 2 AS 'id', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '<value 2>' AS 'value', '<extra field 2>' AS 'name', 1 AS 'printable'\n"\
                "UNION ALL\n"\
                "SELECT 3 AS 'id', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '<value 3>' AS 'value', '<extra field 3>' AS 'name', 0 AS 'printable'\n"\
                "UNION ALL\n"\
                "SELECT 4 AS 'id', 0 AS 'field_id', 0 AS 'repair_id', 0 AS 'item_id', '<value 4>' AS 'value', '<extra field 4>' AS 'name', 1 AS 'printable';");
    while(m_query->next())
    {
        field = std::make_shared<SFieldValueModel>();
        field->load(m_query->record());
        add(field);
    }
}

void SFieldsModel::add(std::shared_ptr<SFieldValueModel> item)
{
    m_valueModels.append(item);
}

void SFieldsModel::setFieldFactory(std::unique_ptr<FieldFactory> factory)
{
    m_fieldFactory = std::move(factory);
    initFieldsHandlers();
}

void SFieldsModel::initFieldsHandlers()
{
    foreach(auto f, m_valueModels)
    {
        if(f->m_field.get())
            continue;

        m_fieldFactory->initFieldHandler(f);
    }

}

void SFieldsModel::setOwnerId(const int id)
{
    m_ownerId = id;
    updateValueModelOwner();
}

void SFieldsModel::updateValueModelOwner()
{
    foreach(auto f, m_valueModels)
    {
        f->setOwnerId(m_ownerId);
    }
}

bool SFieldsModel::commit()
{
    std::shared_ptr<SFieldValueModel> item;
    foreach(item, m_valueModels)
    {
        if(!item->acquireWidgetValue())
        {
            if(item->isPrimaryKeyValid())
            {
                m_removeList.append(item);
            }
            continue;
        }
        item->commit();
    }

    while( !m_removeList.isEmpty() )
    {
        item = m_removeList.last();
        item->delDBRecord();
        m_removeList.removeLast();
    }

    return 1;
}

bool SFieldsModel::validate()
{
    bool ret = 1;

    foreach(auto item, m_valueModels)
    {
        ret &= item->m_entityModel->validate();
    }
    return ret;
}

void SFieldsModel::setAllFailed()
{
    foreach(auto item, m_valueModels)
    {
        if(!item->value().isEmpty())
        {
            item->setFieldsFailed();
            item->i_logRecord->setFieldsFailed();
        }
    }
}

void SFieldsModel::enableEdit(const int cat)
{
    init(cat);
}

int SFieldsModel::printableFieldsCount()
{
    int count = 0;
    foreach(auto item, m_valueModels)
    {
        if(item->isPrintable())
            count++;
    }

    return count;
}

QString SFieldsModel::reportFieldName()
{
    if(m_reportFieldIndex == -1)
        return QString();

    return m_valueModels.at(m_reportFieldIndex)->name();
}

QString SFieldsModel::reportFieldValue()
{
    if(m_reportFieldIndex == -1)
        return QString();

    return m_valueModels.at(m_reportFieldIndex)->value();
}

void SFieldsModel::initValueModel()
{
    initValueModel(QSqlRecord());
}

void SFieldsModel::initValueModel(const QSqlRecord &rec)
{
    std::shared_ptr<SFieldValueModel> model = std::make_shared<SFieldValueModel>(this);
    m_fieldFactory->initFieldHandler(model);
    model->load(rec);
    add(model);
}

void SFieldsModel::initSqlQuery()
{
    initSqlQuery(QSqlDatabase::database(TdConn::main()));
}

void SFieldsModel::initSqlQuery(QSqlDatabase database)
{
    if(!m_query.get())
    {
        m_database = database;
        m_query = std::make_shared<QSqlQuery>(database);
    }
}

/* Метод получения данных для отчетов LimeReport
 * Смотри описание метода с таким же названием в классе SSingleRowJModel
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
        if(m_reportFieldIndex+1 >= m_valueModels.count())
            return;
        while(!m_valueModels.at(++m_reportFieldIndex)->isPrintable())
        {
            if(m_reportFieldIndex+1 >= m_valueModels.count())
                return;
        }
    }
    result = 1;
}

#ifdef QT_DEBUG
void SFieldsModel::randomFill()
{
    for(int j=0; j< m_valueModels.size(); j++)   // автозаполнение обязательных доп. полей
    {
        if ( m_valueModels[j]->property("fieldRequired").toBool() )
            m_valueModels[j]->randomFill();
    }
}
#endif
