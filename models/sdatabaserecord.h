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
protected:
    int i_id = 0;
    bool i_nDBErr = 1;      // флаг обнаружения ошибки выполнения запроса к БД
    bool i_nSrcCodeErr = 1; // флаг обнаружения ошибки на уровне исходного кода
    bool i_nTimeErr = 1;    // флаг обнаружения большой разницы времени сервера и клиента
    QDateTime i_createdUtc;
    bool checkSystemTime();
    bool checkObligatoryFields();
    bool checkTableName();
    QMap<QString, QVariant> i_valuesMap;   // ключ — название столбца (поля) таблицы БД
    QStringList i_obligatoryFields = {};   // поля не имеющие значения по умолчанию
    QSqlQuery *i_query;
    QStringList fields, field_values;
    void fieldsInsFormatter();
    void fieldsUpdFormatter();
    QString fieldValueHandler(const QVariant&);
    bool insert(bool flush = true);
    bool update();
    bool del();
    void dbErrFlagHandler(bool);
    QString tableName;
private:
signals:

};

#endif // SDATABASERECORD_H
