#ifndef SDATABASERECORD_H
#define SDATABASERECORD_H

#include <QObject>
#include <QMetaObject>
#include <QMap>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include "global.h"
#include "com_sql_queries.h"
#include "models/ssqlquerymodel.h"

class SDatabaseRecord : public SSqlQueryModel
{
    Q_OBJECT
    friend class SComRecord;
public:
    explicit SDatabaseRecord(QObject *parent = nullptr);
    ~SDatabaseRecord();
    int lastInsertId();
    QDateTime createdUtc();
    QString created();
    bool isError();
    QString lastError();
protected:
    int i_id = 0;
    bool i_nErr = 1;
    QString i_tableName;
    QStringList i_obligatoryFields = {};   // поля не имеющие значения по умолчанию
    QDateTime i_createdUtc;
    QSqlQuery *i_query;
    QString i_lastError;
    QString fieldValueHandler(const QVariant&);
    bool insert(bool flush = true);
    bool update();
    bool del();
    QMap<QString, QVariant> i_valuesMap;   // ключ — название столбца (поля) таблицы БД
private:
    QStringList fields, field_values;
    bool checkSystemTime();
    bool checkObligatoryFields();
    bool checkTableName();
    void fieldsInsFormatter();
    void fieldsUpdFormatter();
    void dbErrFlagHandler(bool);
signals:

};

#endif // SDATABASERECORD_H
