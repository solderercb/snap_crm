#include "sdatabaserecord.h"
#include "global.h"

SDatabaseRecord::SDatabaseRecord(QObject *parent) : SSqlQueryModel(parent)
{
    i_query = new QSqlQuery(QSqlDatabase::database("connThird"));
}

SDatabaseRecord::~SDatabaseRecord()
{
    delete i_query;
}

int SDatabaseRecord::lastInsertId()
{
    return i_id;
}

/*  Проверка валидности системного времени.
 *  Сравнивает время клиента с сервером
*/
bool SDatabaseRecord::checkSystemTime()
{
    QSqlQuery serverTime(QSqlDatabase::database("connMain"));
    QDateTime date;
    int secDiff;
    QString error;

    serverTime.exec("SELECT UTC_TIMESTAMP();");
    serverTime.first();
    date = serverTime.record().value(0).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    secDiff = date.secsTo(QDateTime::currentDateTimeUtc());
    if( secDiff > 30 || secDiff < -30 )
    {
        error = QString("[Warning] client machine time %1 (UTC), server time %2 (UTC)").arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss")).arg(date.toString("yyyy-MM-dd hh:mm:ss"));
        appLog->appendRecord(error);
        throw Global::ThrowType::TimeError;
    }
    return 1;
}

/*  Проверка обязательных полей (полей, не имеющих значения по умолчанию или связанных с другой таблицей)
 *  Возвращает 1 если всё хорошо.
 */
bool SDatabaseRecord::checkObligatoryFields()
{
    QString error;

    if(i_obligatoryFields.isEmpty())
        qDebug() << this->metaObject()->className() << QString("[WARNING]: Obligatory fields not defined!");

    QStringList::const_iterator i;
    for (i = i_obligatoryFields.constBegin(); i != i_obligatoryFields.constEnd(); ++i)
    {
        if(!i_valuesMap.contains(*i))
        {
            i_nErr = 0;
            error = QString("[Error] ASSERT failure in %1: i_valuesMap[\"%2\"] not set").arg(this->metaObject()->className()).arg(*i);
            appLog->appendRecord(error);
            errorMsg(error);
#ifdef QT_DEBUG
            Q_ASSERT_X(0, this->metaObject()->className(), QString("i_valuesMap[\"%1\"] not set").arg(*i).toLocal8Bit());
#endif
        }
    }
    return i_nErr;
}

bool SDatabaseRecord::checkTableName()
{
    QString error;

    if(i_tableName.isEmpty())
    {
        i_nErr = 0;
        error = QString("[Error] ASSERT failure in %1: variable i_tableName (QString) not set").arg(this->metaObject()->className());
        appLog->appendRecord(error);
        errorMsg(error);
#ifdef QT_DEBUG
        Q_ASSERT_X(0, this->metaObject()->className(), error.toLocal8Bit());
#endif
    }
    return i_nErr;
}

QDateTime SDatabaseRecord::createdUtc()
{
    return i_createdUtc;
}

QString SDatabaseRecord::created()
{
    return localDateTime(createdUtc()).toString("dd.MM.yyyy hh:mm:ss");
}

/* Дефолтный метод установки метки времени для записи в таблице.
 * В большинстве таблиц поле называется created.
*/
void SDatabaseRecord::setCreated(const QDateTime &timestamp)
{
    i_createdUtc = timestamp.toUTC();
    i_valuesMap.insert("created", timestamp);
}

/* Получение id для новой записи в таблице
*/
void SDatabaseRecord::findNewId()
{
    if(!m_isIdColumnNameSet)
    {
        if(i_idColumnName.isEmpty())
            return;
        else
            m_isIdColumnNameSet = 1;
    }

    m_newId = 0;

    QUERY_NEW_ID(i_idColumnName, i_tableName, i_query, i_nErr, m_newId);

    if(!m_newId && i_nErr)
    {
        if(i_query->value(1).toInt() == 0)
            m_newId = 1;
    }

    if(m_newId)
        i_valuesMap.insert(i_idColumnName, m_newId);
}

/*  Формирует и отправляет запрос INSERT
 *  Передварительно производится проверка системного времени и обязательных полей
 *  Возвращает 0 в случае какой-либо ошибки
 */
bool SDatabaseRecord::  insert(bool flushValues)
{
    i_nErr = 1;
    QString q;

#ifdef QT_DEBUG
    checkTableName();
#endif
    if(!checkSystemTime())
        return 0;

    if(!checkObligatoryFields())
        return 0;

    findNewId();

    fieldsInsFormatter();
    q = QString("INSERT INTO `%1` (\n  %2\n) VALUES (\n  %3\n);")\
                                         .arg(i_tableName)\
                                         .arg(fields.join(','))\
                                         .arg(field_values.join(','));
//    qDebug().noquote() << q;
    QUERY_EXEC(i_query,i_nErr)(q);

    if(m_newId && i_nErr)
        i_id = m_newId;
    else
        QUERY_LAST_INS_ID(i_query,i_nErr,i_id);

    dbErrFlagHandler(flushValues);

    if(i_nErr)
        emit modelUpdated();

    return i_nErr;
}

bool SDatabaseRecord::update()
{
    i_nErr = 1;
    if(i_valuesMap.isEmpty())
        return 1;

    QString q;

#ifdef QT_DEBUG
    checkTableName();
#endif
    if(!checkSystemTime())
        return 0;

    fieldsUpdFormatter();
    q = QString("UPDATE\n  `%1`\nSET\n  %2\nWHERE `id` = %3;")\
                                         .arg(i_tableName)\
                                         .arg(fields.join(",\n  "))\
                                         .arg(i_id);

    QUERY_EXEC(i_query,i_nErr)(q);
    dbErrFlagHandler(true);

    if(i_nErr)
        emit modelUpdated();

    return i_nErr;
}

bool SDatabaseRecord::del()
{
    QString q;
    i_nErr = 1;

#ifdef QT_DEBUG
    checkTableName();
#endif
    q = QString("DELETE FROM `%1` WHERE `id` = %2;")\
                                         .arg(i_tableName)\
                                         .arg(i_id);
    //    qDebug().noquote() << q;
    QUERY_EXEC(i_query,i_nErr)(q);
    dbErrFlagHandler(false);

    return i_nErr;
}

bool SDatabaseRecord::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        insert();
    }

    if(!i_nErr)
        throw Global::ThrowType::QueryError;

    return i_nErr;
}

void SDatabaseRecord::dbErrFlagHandler(bool flushValues)
{
    if(!i_nErr)
    {
        errorToLog(metaObject()->className(), i_query->lastError().text());
    }

    if(i_nErr && flushValues)
        i_valuesMap.clear();
}


