#include "sdatabaseauxiliary.h"
#include "global.h"

QDate SDatabaseAuxiliary::localDate(const QDate &utcDate)
{
    return localDateTime(utcDate.startOfDay(QTimeZone::utc())).date();
}

QDate SDatabaseAuxiliary::localDate(const QVariant &utcDate)
{
    return localDate(utcDate.toDate());
}

QDate SDatabaseAuxiliary::utcDate(const QDate &localDate)
{
    return utcDateTime(localDate.startOfDay()).date();
}

QDate SDatabaseAuxiliary::utcDate(const QVariant &localDate)
{
    return utcDate(localDate.toDate());
}

QDateTime SDatabaseAuxiliary::localDateTime(QDateTime utcDateTime)
{
    utcDateTime.setTimeZone(QTimeZone::utc());
    return utcDateTime.toLocalTime();
}

QDateTime SDatabaseAuxiliary::localDateTime(const QVariant &utcDateTime)
{
    return localDateTime(utcDateTime.toDateTime());
}

QDateTime SDatabaseAuxiliary::utcDateTime(QDateTime localDateTime)
{
    localDateTime.setTimeZone(QTimeZone::systemTimeZone());
    return localDateTime.toUTC();
}

QDateTime SDatabaseAuxiliary::utcDateTime(const QVariant &localDateTime)
{
    return utcDateTime(localDateTime.toDateTime());
}

void SDatabaseAuxiliary::errorToLog(const QString &className, const QString &errorText)
{
    QString error;

    error = QString("Query error in %1: \"%2\"").arg(className, errorText);
    appLog->appendRecord(error);
    errorMsg(error);
}

void SDatabaseAuxiliary::errorMsg(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}

QString SDatabaseAuxiliary::fieldValueHandler(const QVariant &value)
{
    QString str_value;
    if(value.isNull())
        str_value = "NULL";
    else if(value.typeName() == QString("QString") || value.typeName() == QString("QByteArray"))
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

void SDatabaseAuxiliary::fieldsInsFormatter()
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

void SDatabaseAuxiliary::fieldsUpdFormatter()
{
    QMap<QString, QVariant>::ConstIterator i;

    fields.clear();
    field_values.clear();
    for (i = i_valuesMap.constBegin(); i != i_valuesMap.constEnd(); ++i)
    {
        fields.append('`' + i.key() + "` = " + fieldValueHandler(i.value()));
    }
}

/*  Возвращает или кэшированное значение поля текущей записи БД
 *  или значение из переменной, ссылка на которую передана вторым параметром, если
 *  кэшированного значения нет.
*/
QVariant SDatabaseAuxiliary::cachedValue(const QString &fieldName, const QVariant &uncashedValue)
{
    if(i_valuesMap.contains(fieldName))
        return i_valuesMap.value(fieldName);

    return uncashedValue;
}

