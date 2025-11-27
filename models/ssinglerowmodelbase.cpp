#include "ssinglerowmodelbase.h"
#include <QTimeZone>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <ProjectGlobals>
#include <QDebug>

/* Иницилизация QSqlRecord при создании объекта.
 * Названия полей будут использоваться для формирования запросов
 * Может быть переопределён.
*/
void SSingleRowModelBase::insertNewField(const int, const QString &name)
{
    QSqlField f;
    f.setName(name);
    m_record->append(f);
}

inline bool SSingleRowModelBase::isEqual(const QVariant &newData, const QVariant &oldData)
{
    return newData == oldData && newData.isNull() == oldData.isNull();
}

/* Возвращает ссылку на объект в кэше.
 * Если объект не существует, то инициализирует новый.
*/
std::shared_ptr<SSingleRowModelBase::ModifiedField> &SSingleRowModelBase::cacheRef(const int index)
{
    std::shared_ptr<ModifiedField> &f = cache[index];
    if(!f.get())
        f = std::make_shared<ModifiedField>();

    return f;
}

/* Значение, загруженное из БД или изменённое (из кэша) */
QVariant SSingleRowModelBase::data(const int index, const int state) const
{
    QVariant v;
    if (!isIndexValid(index))
        return v;

    const auto f = cache.constFind(index);
    if (f != cache.constEnd() && ((*f)->state() & state))
        return (*f)->data();

    int dbi = dbFieldIndex(index);
    if(dbi >= 0)
        return loadedValue(dbi);

    return v;
}

/* Значение, гарантированно соответствующее текущему значению в БД */
std::optional<QVariant> SSingleRowModelBase::commitedData(const int &index) const
{
    if (!isIndexValid(index))
        return std::nullopt;

    const auto f = cache.constFind(index);
    if (f != cache.constEnd())
    {
        if((*f)->state() & (ModifiedField::Executed | ModifiedField::Commited))
            return (*f)->data();
        else if ((*f)->state() & ModifiedField::Updated && (*f)->prev().get())
            return (*f)->prev()->data();
    }

    int dbi = dbFieldIndex(index);
    if((dbi >= 0) && isModelLoaded())
        return loadedValue(dbi);

    return std::nullopt;
}

bool SSingleRowModelBase::setData(const int index, const QVariant &value)
{
    if (!isIndexValid(index))
        return false;

    QVariant newValue = value;
    signalDataToBeChanged(index, newValue);

    std::shared_ptr<ModifiedField> &f = cacheRef(index);

    if(f->state() & ModifiedField::Updated)
    {
       if(isEqual(newValue, f->data()))
           return true;

       if(f->prev().get() && isEqual(newValue, f->prev()->data()))
           f = f->prev();
       else
           f->setData(newValue);

       signalDataChanged(index, newValue);
       return true;
    }

    const QVariant oldValue = data(index, ModifiedField::Executed|ModifiedField::Commited);
    if(!isModelLoaded())
    {
        if(!(f->state() & (ModifiedField::Executed|ModifiedField::Commited)))
        {
            f->setData(newValue);
            f->setState(ModifiedField::Updated);
            signalDataChanged(index, newValue);
            return true;
        }
    }

    if(isEqual(newValue, oldValue))
        return true;

    if (f->state() & (ModifiedField::None|ModifiedField::Executed|ModifiedField::Commited))
    {
        if(f->state() & ModifiedField::None)
            f->setData(oldValue);

        f = std::make_shared<ModifiedField>(ModifiedField::Updated, f);
    }

    f->setData(newValue);
    f->setState(ModifiedField::Updated);
    signalDataChanged(index, newValue);

    return true;
}

QDate SSingleRowModelBase::localDate(const QDate &utcDate)
{
    return localDateTime(utcDate.startOfDay(QTimeZone::utc())).date();
}

QDate SSingleRowModelBase::localDate(const QVariant &utcDate)
{
    return localDate(utcDate.toDate());
}

QDate SSingleRowModelBase::utcDate(const QDate &localDate)
{
    return utcDateTime(localDate.startOfDay()).date();
}

QDate SSingleRowModelBase::utcDate(const QVariant &localDate)
{
    return utcDate(localDate.toDate());
}

QDateTime SSingleRowModelBase::localDateTime(const QDateTime &utcDateTime)
{
    QDateTime t = utcDateTime;
    t.setTimeZone(QTimeZone::utc());
    return t.toLocalTime();
}

QDateTime SSingleRowModelBase::localDateTime(const QVariant &utcDateTime)
{
    return localDateTime(utcDateTime.toDateTime());
}

QDateTime SSingleRowModelBase::utcDateTime(QDateTime localDateTime)
{
    localDateTime.setTimeZone(QTimeZone::systemTimeZone());
    return localDateTime.toUTC();
}

QDateTime SSingleRowModelBase::utcDateTime(const QVariant &localDateTime)
{
    return utcDateTime(localDateTime.toDateTime());
}

QString SSingleRowModelBase::fieldValueHandler(const QVariant &value)
{
    if(value.isNull())
        return "NULL";

    QString str_value;
    switch (value.type())
    {
        case QVariant::Invalid: str_value = "NULL"; break;
        case QVariant::String:
        case QVariant::ByteArray:
        case QVariant::UserType: str_value = "'" + value.toString().replace('\'',"\\\'") + "'"; break;
        case QVariant::DateTime: {
            QDateTime dt = value.toDateTime();
            if(dt.timeZone() != QTimeZone::utc())
            {
                dt.setTimeZone(QTimeZone::systemTimeZone());
                dt =  dt.toUTC();
            }
            str_value =  "'" + dt.toString("yyyy-MM-dd hh:mm:ss") + "'";
            break;
        }
        case QVariant::Date: str_value =  "'" + value.toDate().toString("yyyy-MM-dd") + "'"; break;
        case QVariant::Bool:
        case QVariant::Int: str_value = QString::number(value.toInt()); break;
        case QVariant::Double: str_value = QString::number(value.toDouble(), 'f', 4); break;
        default: str_value = value.toString(); break;
    }

    return str_value;
}

/* Загрузка всех полей записи с заданным первичным ключом
 * Названия полей будут взяты из текущего объекта m_record, после успешного
 * выполнения запроса этот объект будет заменён.
*/
void SSingleRowModelBase::load()
{
    if(!primaryKeyData().isValid())
        return;

    initSqlQuery();

    i_query->exec(constructSelectQuery());

    if(!i_query->first())
        loadError(Global::ThrowType::ResultError);

    m_record = std::make_shared<QSqlRecord>(i_query->record());
    convertTimestampFieldsToLocal();
    cache.clear();  // очистка кэша в конце, т. к. в нём может храниться primaryKey
}

/* Инициализация поля модели значением, загруженным альтернативными способами (например,
 * из модели таблицы)
*/
void SSingleRowModelBase::initFieldWithPrevLoaded(const int index, const QVariant &value)
{
    if(index < 0)
        return;

    m_record->setValue(index, value);
}

void SSingleRowModelBase::loadError(const int type)
{
    Global::throwError(type);
}

int SSingleRowModelBase::indexOfCreated()
{
    return fieldIndex("created");
}

/* В БД все таймштампы в UTC, в коде — локальное время.
 * Если виджет обновляется значением модели, которое было задано ранее, а
 * модель при этом не перезагружалась, то createdStr() вернёт неправильное время.
 * Этот метод должен вызываться только при загрузке данных из БД.
*/
void SSingleRowModelBase::convertTimestampFieldsToLocal()
{
    int index = indexOfCreated();
    if(index == -1)
        return;

    m_record->setValue(index, localDateTime(m_record->value(index).toDateTime()));
}

std::shared_ptr<QSqlDatabase> SSingleRowModelBase::database() const
{
    return m_database;
}

void SSingleRowModelBase::setDatabase(const QSqlDatabase &database)
{
    m_database = std::make_shared<QSqlDatabase>(database);
    i_query = std::make_unique<QSqlQuery>(*(m_database.get()));
}

void SSingleRowModelBase::initSqlQuery()
{
    initSqlQuery(QSqlDatabase::database("connThird"));
}

void SSingleRowModelBase::initSqlQuery(QSqlDatabase database)
{
    if(i_query)
        return;

    if(!m_database)
        m_database = std::make_shared<QSqlDatabase>(database);

    i_query = std::make_unique<QSqlQuery>(*(m_database.get()));
}

QString SSingleRowModelBase::fieldsForSelectQuery()
{
    QString fields;
    for(int i = 0; i < rec()->count(); i++)
    {
//        if(i == primaryKey())
//            continue;

        if(!fields.isEmpty())
            fields.append(",\n");

        fields.append(fieldForSelectQuery(i));
    }

    return fields;
}

QString SSingleRowModelBase::wrapSelectQueryFields(const QString &fields)
{
    QString query = fields;
    query.prepend("SELECT\n");
    query.append(QString("\nFROM `%1` WHERE `%2` = %3;")
                 .arg(table())
                 .arg(rec()->fieldName(primaryKeyIndex()))
                 .arg(fieldValueHandler(primaryKeyData())));

    return query;
}

QString SSingleRowModelBase::constructSelectQuery()
{
    return wrapSelectQueryFields(fieldsForSelectQuery());
}

/* Проверка текущего элемента кэша на соответствие условиям пакетной обработки.
 * Метод базового класса всегда возвращает 0 (поле не пропускается)
*/
bool SSingleRowModelBase::skipField(CacheMap::ConstIterator f)
{
    return dbFieldIndex(f.key()) < 0;
}

/* Проверка кэша на наличие изменённых данных.
*/
bool SSingleRowModelBase::isDirty()
{
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i))
            continue;

//        if(i.key() == primaryKeyIndex())
//            continue;

        if(isFieldDirty(i))
            return true;
    }
    return false;
}

bool SSingleRowModelBase::isFieldDirty(const int index)
{
    auto f = cache.constFind(index);
    if(f == cache.constEnd() || !((*f)->state() & (ModifiedField::Updated | ModifiedField::Failed)))
        return 0;

    return 1;
}

QString SSingleRowModelBase::constructInsertQuery()
{
    QString fields;
    QString values;
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i) || !isFieldDirty(i))
            continue;

        if(!fields.isEmpty())
            fields.append(",\n");
        fields.append("  `" + rec()->fieldName(i.key()) + '`');

        if(!values.isEmpty())
            values.append(",\n");
        values.append("  " + fieldValueHandler((*i)->data()));
    }

    QString q = QString("INSERT INTO `%1`(\n%2\n) VALUES (\n%3\n);").arg(table(), fields, values);
    //    qDebug().noquote() << q;
    return q;
}

QString SSingleRowModelBase::prepareQueryUpdatePairs()
{
    QString setPairs;
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i) || !isFieldDirty(i))
            continue;

        int index = i.key();
        if(index == primaryKeyIndex())
            continue;

        if(!setPairs.isEmpty())
            setPairs.append(",\n");
        setPairs.append("  `" + dbFieldName(index) + "` = " + fieldValueHandler((*i)->data()));
    }

    return setPairs;
}

QString SSingleRowModelBase::wrapUpdateQueryPairs(const QString &setPairs)
{
    QString q = QString("UPDATE\n  `%1`\nSET\n%2\nWHERE `%3` = %4;").arg(
                            table(),
                            setPairs,
                            rec()->fieldName(primaryKeyIndex()))
                            .arg(fieldValueHandler(primaryKeyData()));

    return q;
}

QString SSingleRowModelBase::constructUpdateQuery()
{
    return wrapUpdateQueryPairs(prepareQueryUpdatePairs());
}

/* Запись значения value непосредственно в кэш.
 * Метод предназначен для сохранения значения в кэш без дополнительных действий по сигналу dataChanged;
 * с помощью newState можно сразу задать состояние, по умолчанию Updated.
 * Пример: в классе SComSettings использутеся для парсинга данных, представленных в виде JSON, и сохранения
 * отдельных значений в поля класса, не сопоставленные с полями таблицы БД.
*/
void SSingleRowModelBase::setCacheData(const int index, const QVariant &value, ModifiedField::State newState)
{
    std::shared_ptr<ModifiedField> &f = cacheRef(index);
    f->setData(value);
    f->setState(newState);
}

void SSingleRowModelBase::setState(const int index, ModifiedField::State state)
{
    auto f = cache.constFind(index);
    if(f == cache.constEnd())
        return;

    setState(*f, state);
}

void SSingleRowModelBase::setState(std::shared_ptr<ModifiedField> field, ModifiedField::State state)
{
    if(field->state() == ModifiedField::None)
        return;

    field->setState(state);
    if(state & (ModifiedField::Executed|ModifiedField::Commited))
        field->prev().reset();

}

void SSingleRowModelBase::setAllState(ModifiedField::State state)
{
    CacheMap::iterator i = cache.begin();
    CacheMap::iterator e = cache.end();
    for (; i != e; ++i)
    {
        if(skipField(i) || (*i)->state()&ModifiedField::Commited)   // статус Commited изменять нельзя
            continue;

        setState(*i, state);
    }
}

/* Установка статуса Commited всем элементам кэша.
 * Статус указывает, что данные гарантированно записаны в БД (транзакция закрыта запросом COMMIT).
 * По значимости он равносилен Executed, но в отдельных сценариях может быть использован для обработки
 * сбоев без перезагрузки данных из БД. Однако, перезагрузка надёжнее и универсальнее (например, в базе
 * могут быть триггеры).
 *
 */
void SSingleRowModelBase::setFieldsCommited()
{
    setAllState(ModifiedField::Commited);
}

void SSingleRowModelBase::setFieldsExecuted()
{
    setAllState(ModifiedField::Executed);
}

void SSingleRowModelBase::setFieldsFailed()
{
    setAllState(ModifiedField::Failed);
}

void SSingleRowModelBase::reportData(const int index, QVariant &data)
{
    data = this->data(index);
}

void SSingleRowModelBase::reportData(const QString &field, QVariant &data)
{
    int id = m_namesMap->key(field, -1);
    if(id >= 0)
        data = this->data(id);
    else
        reportMetaData(field, data);
}

/* Виртуальный метод, передающий через data значение дополнительного поля отчета.
 * Переопределён в наследующих классах, т. к. использует мета-систему.
*/
void SSingleRowModelBase::reportMetaData(const QString&, QVariant&)
{
}
