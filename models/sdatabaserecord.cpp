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
        return 0;

    return 1;
}

bool SDatabaseRecord::checkObligatoryFields()
{
    if(i_obligatoryFields.isEmpty())
        qDebug() << this->metaObject()->className() << QString("[WARNING]: Obligatory fields not defined!");

    QStringList::const_iterator i;
    for (i = i_obligatoryFields.constBegin(); i != i_obligatoryFields.constEnd(); ++i)
    {
        Q_ASSERT_X(i_valuesMap.contains(*i), this->metaObject()->className(), QString("i_valuesMap[\"%1\"] not set").arg(*i).toLocal8Bit());
    }
    return 0;
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

bool SDatabaseRecord::insert(const QString &table)
{
    QString q;

    checkSystemTime();
    checkObligatoryFields();
    fieldsInsFormatter();
    q = QString("INSERT INTO `%1` (\n  %2\n) VALUES (\n  %3\n);")\
                                         .arg(table)\
                                         .arg(fields.join(','))\
                                         .arg(field_values.join(','));
//    qDebug().noquote() << q;
    QUERY_EXEC(i_query,i_nDBErr)(q);
    QUERY_LAST_INS_ID(i_query,i_nDBErr,i_id);
    if(i_nDBErr)
        i_valuesMap.clear();

    return i_nDBErr;
}

bool SDatabaseRecord::update(const QString &table, const int id)
{
    QString q;

    checkSystemTime();
    fieldsUpdFormatter();
    q = QString("UPDATE\n  `%1`\nSET\n  %2\nWHERE `id` = %3;")\
                                         .arg(table)\
                                         .arg(fields.join(",\n  "))\
                                         .arg(id);
//    qDebug().noquote() << q;
    QUERY_EXEC(i_query,i_nDBErr)(q);
    if(i_nDBErr)
        i_valuesMap.clear();

    return i_nDBErr;
}

bool SDatabaseRecord::del(const QString &table, const int id)
{
    QString q;

    q = QString("DELETE FROM `%1` WHERE `id` = %2;")\
                                         .arg(table)\
                                         .arg(id);
    QUERY_EXEC(i_query,i_nDBErr)(q);
//    qDebug().noquote() << q;

    return i_nDBErr;
}
