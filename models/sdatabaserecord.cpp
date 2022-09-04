#include "sdatabaserecord.h"

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

    serverTime.exec("SELECT UTC_TIMESTAMP();");
    serverTime.first();
    date = serverTime.record().value(0).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    secDiff = date.secsTo(QDateTime::currentDateTimeUtc());
    if( secDiff > 30 || secDiff < -30 )
    {
        i_nErr = 0;
        i_lastError = QString("[Warning] client machine time %1 (UTC), server time %2 (UTC)").arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss")).arg(date.toString("yyyy-MM-dd hh:mm:ss"));
        appLog->appendRecord(i_lastError);
    }

    return i_nErr;
}

/*  Проверка обязательных полей (полей, не имеющих значения по умолчанию или связанных с другой таблицей)
 *  Возвращает 1 если всё хорошо.
 */
bool SDatabaseRecord::checkObligatoryFields()
{
    if(i_obligatoryFields.isEmpty())
        qDebug() << this->metaObject()->className() << QString("[WARNING]: Obligatory fields not defined!");

    QStringList::const_iterator i;
    for (i = i_obligatoryFields.constBegin(); i != i_obligatoryFields.constEnd(); ++i)
    {
        if(!i_valuesMap.contains(*i))
        {
            i_nErr = 0;
            i_lastError = QString("[Error] ASSERT failure in %1: i_valuesMap[\"%2\"] not set").arg(this->metaObject()->className()).arg(*i);
            appLog->appendRecord(i_lastError);
        }
#ifdef QT_DEBUG
        Q_ASSERT_X(i_valuesMap.contains(*i), this->metaObject()->className(), QString("i_valuesMap[\"%1\"] not set").arg(*i).toLocal8Bit());
#endif
    }
    return i_nErr;
}

bool SDatabaseRecord::checkTableName()
{
    if(i_tableName.isEmpty())
    {
        i_nErr = 0;
        i_lastError = QString("[Error] ASSERT failure in %1: variable i_tableName (QString) not set").arg(this->metaObject()->className());
        appLog->appendRecord(i_lastError);
        Q_ASSERT_X(0, this->metaObject()->className(), i_lastError.toLocal8Bit());
        return 0;
    }
    return 1;
}

QDateTime SDatabaseRecord::createdUtc()
{
    return i_createdUtc;
}

QString SDatabaseRecord::created()
{
    QDateTime date = i_createdUtc;
    date.setTimeZone(QTimeZone::utc());

    return date.toLocalTime().toString("dd.MM.yyyy hh:mm:ss");
}

void SDatabaseRecord::fieldsInsFormatter()
{
    QMap<QString, QVariant>::ConstIterator i;

    fields.clear();
    field_values.clear();
    for (i = i_valuesMap.constBegin(); i != i_valuesMap.constEnd(); ++i)
    {
        fields.append('`' + i.key() + '`');
        field_values.append(fieldValueHandler(i.value()));
    }

}

void SDatabaseRecord::fieldsUpdFormatter()
{
    QMap<QString, QVariant>::ConstIterator i;

    fields.clear();
    field_values.clear();
    for (i = i_valuesMap.constBegin(); i != i_valuesMap.constEnd(); ++i)
    {
        fields.append('`' + i.key() + "` = " + fieldValueHandler(i.value()));
    }
}

QString SDatabaseRecord::fieldValueHandler(const QVariant &value)
{
    QString str_value;
    if(value.isNull())
        str_value = "NULL";
    else if(value.typeName() == QString("QString"))
        str_value = "'" + value.toString().replace('\'',"\\\'") + "'";
    else if(value.typeName() == QString("QDateTime"))
    {
        QDateTime date = value.toDateTime();
        date.setTimeZone(QTimeZone::systemTimeZone());
        str_value =  date.toUTC().toString("yyyy-MM-dd hh:mm:ss");
        str_value =  "'" + str_value + "'";
    }
    else if(value.typeName() == QString("QDate"))
    {
        str_value =  value.toDate().toString("yyyy-MM-dd");
        str_value =  "'" + str_value + "'";
    }
    else if(value.typeName() == QString("float"))
        str_value =  QString::number(value.toFloat(), 'f', 4);
    else
        str_value = value.toString();

    return str_value;
}

/*  Формирует и отправляет запрос INSERT
 *  Передварительно производится проверка системного времени и обязательных полей
 *  Возвращает 0 в случае какой-либо ошибки
 */
bool SDatabaseRecord::insert(bool flushValues)
{
    QString q;

#ifdef QT_DEBUG
    checkTableName();
#endif
    if(!checkSystemTime())
        return 0;

    if(!checkObligatoryFields())
        return 0;

    fieldsInsFormatter();
    q = QString("INSERT INTO `%1` (\n  %2\n) VALUES (\n  %3\n);")\
                                         .arg(i_tableName)\
                                         .arg(fields.join(','))\
                                         .arg(field_values.join(','));
//    qDebug().noquote() << q;
    QUERY_EXEC(i_query,i_nErr)(q);
    QUERY_LAST_INS_ID(i_query,i_nErr,i_id);
    dbErrFlagHandler(flushValues);

    return i_nErr;
}

bool SDatabaseRecord::update()
{
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

    return i_nErr;
}

bool SDatabaseRecord::del()
{
    QString q;

    checkTableName();
    q = QString("DELETE FROM `%1` WHERE `id` = %2;")\
                                         .arg(i_tableName)\
                                         .arg(i_id);
    //    qDebug().noquote() << q;
    QUERY_EXEC(i_query,i_nErr)(q);
    dbErrFlagHandler(false);

    return i_nErr;
}

void SDatabaseRecord::dbErrFlagHandler(bool flushValues)
{
    if(!i_nErr)
    {
        i_lastError = QString("Query error in %1: \"%2\"").arg(this->metaObject()->className()).arg(i_query->lastError().text());
        appLog->appendRecord(i_lastError);
    }

    if(i_nErr && flushValues)
        i_valuesMap.clear();
}

bool SDatabaseRecord::isError()
{
    return !i_nErr;
}

QString SDatabaseRecord::lastError()
{
    return i_lastError;
}
