#include "ssinglerowmodel.h"
#include <QTimeZone>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <QDebug>

SSingleRowModel::SSingleRowModel(QObject *parent) : QObject(parent)
{
    m_namesMap = std::make_unique<QMap<int, QString>>();
    m_reportFieldsMap = std::make_unique<QHash<QString, QString>>();
    m_record = std::make_shared<QSqlRecord>();
    connect(this, &SSingleRowModel::dataChanged, this, &SSingleRowModel::logDataChange);
}

SSingleRowModel::~SSingleRowModel()
{
}

void SSingleRowModel::setTable(const QString &table)
{
    i_tableName = table;
}

void SSingleRowModel::setPrimaryKeyIndex(const int index)
{
    i_primaryKeyIndex = index;
}

void SSingleRowModel::setPrimaryKey(const QVariant &data)
{
    std::shared_ptr<ModifiedField> &f = cacheRef(i_primaryKeyIndex);
    f->setData(data);
    f->setState(ModifiedField::Executed);
}

void SSingleRowModel::setEditStrategy(int strategy)
{
    m_editStrategy = strategy;
}

void SSingleRowModel::setQueryIdBeforeInsert(bool state)
{
    m_queryIdBeforeInsert = state;
}

int SSingleRowModel::flags(const int index)
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable;
}

bool SSingleRowModel::setData(const int index, const QVariant &data)
{
    if (!(flags(index) & Qt::ItemIsEditable))
        return false;

    SSingleRowModelBase::setData(index, data);

    if(m_editStrategy == OnFieldChange)
    {
        // немедленная запись данных в БД только в случае изменения;
        // если модель "пустая" (т. е. данные не загружались из БД), то, вероятнее всего,
        // при попытке записи произойдёт сбой, т. к. не заданы данные обязательных полей
        if(index != i_primaryKeyIndex && isPrimaryKeyValid())
            commit();
    }

    return true;
}

void SSingleRowModel::queryNewId()
{
    initSqlQuery();

    QUERY_EXEC_TH(i_query, 1, QUERY_NEW_ID(m_record->fieldName(i_primaryKeyIndex), i_tableName));

    i_query->first();
    QVariant val = i_query->value("id");

    if(val.isNull())
    {
        if(i_query->value("count").toInt() == 0) // Если таблица не содержит записей
            setData(i_primaryKeyIndex, 1);

        return;
    }

    setData(i_primaryKeyIndex, val);
}

void SSingleRowModel::queryLastInsertId()
{
    initSqlQuery();

    int id = 0;
    QUERY_LAST_INS_ID_TH(i_query,1,id);
    setPrimaryKey(id);
}

void SSingleRowModel::signalDataToBeChanged(const int index, QVariant &data)
{
    emit beginDataChange(index, data);
}

void SSingleRowModel::signalDataChanged(const int index, QVariant &data)
{
    emit dataChanged(index, data);
}

bool SSingleRowModel::insert()
{
    if(!isDirty())
        return 1;

    if(!checkObligatoryFields())
        return 0;

    initSqlQuery();

    if(m_queryIdBeforeInsert)
        queryNewId();

    QUERY_EXEC_TH(i_query, 1, constructInsertQuery());

    // TODO: после успешной записи данных в БД нужно выполнить запрос SELECT для обновления полей;
    // можно только тех, которые были только что записаны, но лучше всех, вдруг в БД добавлены какие-нибудь триггеры
    if(!m_queryIdBeforeInsert)
        queryLastInsertId();

    // TODO: в setData() эмитируется сигнал dataChanged(); сигнал modelUpdated() должен эмитироваться при загрузке данных из БД
//    emit modelUpdated();

    return 1;
}

bool SSingleRowModel::update()
{
    if(!isDirty())
        return 1;

    initSqlQuery();

    QUERY_EXEC_TH(i_query, 1, constructUpdateQuery());

    // TODO: в setData() эмитируется сигнал dataChanged(); сигнал modelUpdated() должен эмитироваться при загрузке данных из БД
//    emit modelUpdated();

    return 1;
}

bool SSingleRowModel::del()
{
    initSqlQuery();

    QString q;

#ifdef QT_DEBUG
    checkTableName();
#endif
    q = QString("DELETE FROM `%1` WHERE `id` = %2;")\
                    .arg(i_tableName)\
                    .arg(data(i_primaryKeyIndex, ModifiedField::Executed|ModifiedField::Commited).toInt());
//    qDebug().noquote() << q;
    QUERY_EXEC_TH(i_query, 1, q);
    setPrimaryKey(QVariant());  // удаление primaryKey

    return 1;
}

bool SSingleRowModel::commit()
{
    if(!isDirty())
        return true;

#ifdef QT_DEBUG
    checkTableName();
#endif
    checkSystemTime();

    if(isPrimaryKeyValid())
    {
        update();
    }
    else
    {
        insert();
    }

    // если не возникло исключения, то все поля помечаются Executed
    setFieldsExecuted();

    return 1;
}

void SSingleRowModel::dbErrFlagHandler(bool)
{
    setAllState(ModifiedField::Failed);
}

bool SSingleRowModel::checkObligatoryFields()
{
    QString error;
    bool nErr = 1;

    if(i_obligatoryFields.isEmpty())
        qDebug() << this->metaObject()->className() << QString("[WARNING]: Obligatory fields not defined!");

    QStringList::const_iterator i;
    for (i = i_obligatoryFields.constBegin(); i != i_obligatoryFields.constEnd(); ++i)
    {
        if(!isFieldDirty(m_record->indexOf(*i)))
        {
            nErr = 0;
            error = QString("[Error] ASSERT failure in %1: mandatory field `%2` not found in cache").arg(this->metaObject()->className()).arg(*i);
            Global::errorMsg(error);
            return nErr;
        }
    }
    return nErr;
}

bool SSingleRowModel::checkTableName()
{
    QString error;
    bool nErr = 1;

    if(i_tableName.isEmpty())
    {
        nErr = 0;
        error = QString("[Error] ASSERT failure in %1: variable i_tableName (QString) not set").arg(this->metaObject()->className());
        Global::errorMsg(error);
#ifdef QT_DEBUG
        Q_ASSERT_X(0, this->metaObject()->className(), error.toLocal8Bit());
#endif
    }
    return nErr;
}

/* Загрузка с эмитом сигналов о начале и завершении загрузки
*/
void SSingleRowModel::load()
{
    beginUpdateModel();

    SSingleRowModelBase::load();

    endUpdateModel();
}

//void SSingleRowModel::setQuery(const QString&)
//{

//}

//void SSingleRowModel::setQuery(const QSqlQuery&)
//{

//}

/* Метод возвращает таймштамп в UTC.
*/
QDateTime SSingleRowModel::createdUtc()
{
    int index = indexOfCreated();
    if(index == -1)
        return QDateTime();

    return data(index).toDateTime().toUTC();
}

QString SSingleRowModel::createdStr()
{
    int index = indexOfCreated();
    if(index == -1)
        return QString();

    return data(index).toDateTime().toString("dd.MM.yyyy hh:mm:ss");
}

bool SSingleRowModel::checkSystemTime()
{
    auto serverTime = std::make_shared<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    QDateTime date;
    int secDiff;
    QString error;

    QUERY_EXEC_TH(serverTime,1,"SELECT UTC_TIMESTAMP();");

    serverTime->first();
    date = serverTime->record().value(0).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    secDiff = date.secsTo(QDateTime::currentDateTimeUtc());
    if( secDiff > 30 || secDiff < -30 )
    {
        error = QString("[Error] client machine time %1 (UTC), server time %2 (UTC)")
                .arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(date.toString("yyyy-MM-dd hh:mm:ss"));
        Global::throwError(Global::ThrowType::TimeError, error);
    }
    return 1;
}

/* Возвращает кол-во не пустых элементов в кэше (статус которых не None)
 * Метод нужен для тестов
*/
int SSingleRowModel::cacheSize()
{
    int size = 0;
    CacheMap::iterator i = cache.begin();
    CacheMap::iterator e = cache.end();
    for (; i != e; ++i)
    {
        if((*i)->state() == ModifiedField::None)
            continue;

        size++;
    }

    return size;
}

void SSingleRowModel::clearCache()
{
    cache.clear();
}

void SSingleRowModel::clearEverything()
{
    clearCache();
    for(int i = 0; i < m_record->count(); i++)
    {
        m_record->setValue(i, QVariant());
    }

}

QSqlError SSingleRowModel::lastError()
{
    if(!i_query)
        return QSqlError();

    return i_query->lastError();
}

void SSingleRowModel::beginUpdateModel()
{
    emit modelAboutToBeUpdated();
}

void SSingleRowModel::endUpdateModel()
{
    emit modelUpdated();
}

bool SSingleRowModel::isPrimaryKeyValid() const
{
    return data(i_primaryKeyIndex, ModifiedField::Executed|ModifiedField::Commited).isValid();
}

const QVariant SSingleRowModel::operator[](int index)
{
    return data(index);
}

void SSingleRowModel::reportMetaData(const QString &field, QVariant &data)
{
    metaObject()->invokeMethod(this, m_reportFieldsMap->value(field).toLocal8Bit(), Qt::DirectConnection, Q_RETURN_ARG(QVariant, data));
}
