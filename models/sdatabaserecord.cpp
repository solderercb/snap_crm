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
        i_nErr = 0;
        error = QString("[Warning] client machine time %1 (UTC), server time %2 (UTC)").arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss")).arg(date.toString("yyyy-MM-dd hh:mm:ss"));
        appLog->appendRecord(error);
    }

    return i_nErr;
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

QDateTime SDatabaseRecord::utcToLocal(QDateTime timestamp)
{
    timestamp.setTimeZone(QTimeZone::utc());
    return timestamp.toLocalTime();
}

QDateTime SDatabaseRecord::createdUtc()
{
    return i_createdUtc;
}

QString SDatabaseRecord::created()
{
    return utcToLocal(i_createdUtc).toString("dd.MM.yyyy hh:mm:ss");
}

/* Дефолтный метод установки метки времени для записи в таблице.
 * В большинстве таблиц поле называется created.
*/
void SDatabaseRecord::setCreated(const QDateTime &timestamp)
{
    i_createdUtc = timestamp.toUTC();
    i_valuesMap.insert("created", timestamp);
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
        QDateTime dt = value.toDateTime();
        if(dt.timeZone() != QTimeZone::utc())
        {
            dt.setTimeZone(QTimeZone::systemTimeZone());
            dt =  dt.toUTC();
        }
        str_value =  dt.toString("yyyy-MM-dd hh:mm:ss");
        str_value =  "'" + str_value + "'";
    }
    else if(value.typeName() == QString("QDate"))
    {
        str_value =  value.toDate().toString("yyyy-MM-dd");
        str_value =  "'" + str_value + "'";
    }
    else if(value.typeName() == QString("double"))
        str_value =  QString::number(value.toDouble(), 'f', 4);
    else if(value.typeName() == QString("double"))
        str_value =  QString::number(value.toDouble(), 'f', 4);
    else
        str_value = value.toString();

    return str_value;
}

/*  Формирует и отправляет запрос INSERT
 *  Передварительно производится проверка системного времени и обязательных полей
 *  Возвращает 0 в случае какой-либо ошибки
 */
bool SDatabaseRecord::  insert(bool flushValues)
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
    QString error;

    if(!i_nErr)
    {
        error = QString("Query error in %1: \"%2\"").arg(this->metaObject()->className()).arg(i_query->lastError().text());
        appLog->appendRecord(error);
        errorMsg(error);
    }

    if(i_nErr && flushValues)
        i_valuesMap.clear();
}

void SDatabaseRecord::errorMsg(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}

