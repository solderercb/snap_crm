#ifndef SDATABASEAUXILIARY_H
#define SDATABASEAUXILIARY_H

class SDatabaseAuxiliary
{
protected:
    QMap<QString, QVariant> i_valuesMap;   // ключ — название столбца (поля) таблицы БД
    QStringList fields, field_values;
    void errorToLog(const QString &className, const QString &errorText);
    void errorMsg(const QString&);
    QString fieldValueHandler(const QVariant&);
    void fieldsInsFormatter();
    void fieldsUpdFormatter();
};


#endif // SDATABASEAUXILIARY_H
