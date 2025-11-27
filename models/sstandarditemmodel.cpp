#include "sstandarditemmodel.h"
#include <ProjectGlobals>
#include <SSingleRowJModel>

SStandardItemModel::SStandardItemModel(QObject *parent):
    QStandardItemModel(parent),
    SAbstractItemModel(parent)
{
    abstractItemModel = this;
}

SStandardItemModel::~SStandardItemModel()
{

}

/* Создаёт модель данных из enum
 * _enum - ссылка на данные
 * contextTr - указатель на метод QObject::tr() объекта из которого вызывается данный метод (объекта, в контексте которого задан перевод)
*/
SStandardItemModel *SStandardItemModel::modelFromEnum(QMetaEnum &_enum, QString (*contextTr)(const char *, const char *, int))
{
    SStandardItemModel *list = new SStandardItemModel();
    QList<QStandardItem*> *item;
    if(contextTr == nullptr)
    {
        contextTr = QObject::tr;
    }

    for(int i = 0; i < _enum.keyCount(); i++)
    {
        item = new QList<QStandardItem*>();
        *item << new QStandardItem(contextTr(_enum.key(i), nullptr, -1)) << new QStandardItem(QString::number(_enum.value(i)));
        list->appendRow(*item);
    }
    list->setHorizontalHeaderLabels({"name", "id"});

    return list;
}

bool SStandardItemModel::isSingleRowModelExists(const int row)
{
    return m_modified.contains(row);
}

QVariant SStandardItemModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return QStandardItemModel::data(index, role);

    std::shared_ptr<SSingleRowJModel> model;
    MapperForSingleRowModel* mapper = nullptr;

    model = cacheItem(index.row());

    if(model)
    {
        mapper = dynamic_cast<MapperForSingleRowModel*>(model.get());

        if(mapper)
        {
            int srm_index = -1;
            srm_index = mapper->singleRowModelIndex(index.column());

            if(srm_index >= 0)
            {
                return model->data(srm_index);
            }

            return mapper->extraData(index.column());
        }

        return model->data(index.column()); // для случаев когда модель SSingleRowModel совпадает с моделью данных таблицы (например, комментарии)
    }

    return QStandardItemModel::data(index, role);
}

bool SStandardItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return QStandardItemModel::setData(index, value, role);

    std::shared_ptr<SSingleRowJModel> model;
    MapperForSingleRowModel* mapper = nullptr;
    int row = index.row();

    model = singleRowModel(row);

    if(model)
    {
        bool ret = 1;
        mapper = dynamic_cast<MapperForSingleRowModel*>(model.get());

        if(mapper)
            ret = mapper->setSingleRowModelData(index.column(), value);
        else
            ret = model->setData(index.column(), value);

        if(ret)
            emit dataChanged(index, index, QVector<int>({Qt::DisplayRole, Qt::EditRole}));
        return ret;
    }

    return QStandardItemModel::setData(index, value, role);
}

/* Добавление count строк с позиции row и сдвиг имеющихся.
*/
bool SStandardItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    QList<int> keys = m_modified.keys();
    auto index = keys.crbegin();
    for(; (index != keys.crend()) && (*index >= row); ++index)
    {
        if(!m_modified.contains(*index))
            continue;

        m_modified.insert((*index)+count, m_modified.take(*index));
    }

    bool ret = QStandardItemModel::insertRows(row, count, parent);
    return ret;
}

/* Удаление count строк начиная с row и сдвиг имеющихся.
 * Элементы кэша из указанного диапазона удаляются, никаких запросов к базе
 * не выполняется — это должно быть реализовано в производных моделях.
*/
bool SStandardItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    const int fromRow = row;
    const int toRow = row+count;

    QList<int> keys = m_modified.keys();
    auto i = keys.constBegin();
    while(i != keys.constEnd())
    {
        int key = *(i++);
        if(key < fromRow)
            continue;

        if(key < toRow)
        {
            m_modified.take(key);
        }
        else
        {
            m_modified.insert(key-count, m_modified.take(key));
        }
    }

    return QStandardItemModel::removeRows(row, count, parent);
}

std::shared_ptr<SSingleRowJModel> SStandardItemModel::cacheItem(const int row) const
{
    return m_modified.value(row);
}

void SStandardItemModel::setCacheItem(const int row, std::shared_ptr<SSingleRowJModel> model)
{
    m_modified.insert(row, model);
}

void SStandardItemModel::clear()
{
    SStandardItemModel::clearCache();
    removeRows(0, rowCount());
}

void SStandardItemModel::clearCache()
{
    m_modified.clear();
}

bool SStandardItemModel::isDirty()
{
    foreach (auto rec, m_modified)
    {
        if(rec->isDirty())
            return 1;
    }

    return 0;
}

QSqlDatabase SStandardItemModel::loadConnection() const
{
    if(!m_connection)
        return QSqlDatabase::database("connMain");
    return *(m_connection.get());
}

QSqlDatabase SStandardItemModel::commitConnection() const
{
    if(!m_connection)
        return QSqlDatabase::database("connThird");
    return *(m_connection.get());
}

void SStandardItemModel::setConnection(const QSqlDatabase &conn)
{
    m_connection = std::make_shared<QSqlDatabase>(conn);
}
