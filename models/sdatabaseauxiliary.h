#ifndef SDATABASEAUXILIARY_H
#define SDATABASEAUXILIARY_H

class SDatabaseAuxiliary
{
public:
    static QDate localDate(const QDate &utcDate);
    static QDate localDate(const QVariant &utcDate);
    static QDate utcDate(const QDate &localDate);
    static QDate utcDate(const QVariant &localDate);
    static QDateTime localDateTime(QDateTime utcDateTime);
    static QDateTime localDateTime(const QVariant &utcDateTime);
    static QDateTime utcDateTime(QDateTime localDateTime);
    static QDateTime utcDateTime(const QVariant &localDateTime);
    static QString fieldValueHandler(const QVariant &value);
protected:
    QMap<QString, QVariant> i_valuesMap;   // ключ — название столбца (поля) таблицы БД
    QStringList fields, field_values;
    void errorToLog(const QString &className, const QString &errorText);
    void errorMsg(const QString&);
    void fieldsInsFormatter();
    void fieldsUpdFormatter();
    QVariant cachedValue(const QString &fieldName, const QVariant &uncashedValue);
};


#endif // SDATABASEAUXILIARY_H
