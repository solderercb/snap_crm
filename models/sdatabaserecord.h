#ifndef SDATABASERECORD_H
#define SDATABASERECORD_H

#include <QObject>
#include <QMetaObject>
#include <QMap>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include <QMessageBox>
#include "com_sql_queries.h"
#include "models/ssqlquerymodel.h"

class SDatabaseErrorLogger
{
protected:
    void errorToLog(const QString &className, const QString &errorText);
    void errorMsg(const QString&);
};

class SDatabaseRecord : public SSqlQueryModel, public SDatabaseErrorLogger
{
    Q_OBJECT
    friend class SComRecord;
public:
    explicit SDatabaseRecord(QObject *parent = nullptr);
    ~SDatabaseRecord();
    int lastInsertId();
    QDateTime utcToLocal(QDateTime timestamp);
    QDateTime createdUtc();
    QString created();
    void setCreated(const QDateTime &timestamp);

protected:
    int i_id = 0;
    bool i_nErr = 1;
    QString i_tableName;
    QStringList i_obligatoryFields = {};   // поля не имеющие значения по умолчанию
    QDateTime i_createdUtc;
    QSqlQuery *i_query;
    QString fieldValueHandler(const QVariant&);
    virtual void findNewId();
    bool insert(bool flush = true);
    bool update();
    bool del();
    QMap<QString, QVariant> i_valuesMap;   // ключ — название столбца (поля) таблицы БД
    QString i_idColumnName;
private:
    QStringList fields, field_values;
    int m_newId = 0;
    bool m_isIdColumnNameSet = 0;
    bool checkSystemTime();
    bool checkObligatoryFields();
    bool checkTableName();
    void fieldsInsFormatter();
    void fieldsUpdFormatter();
    void dbErrFlagHandler(bool);
signals:

};

#endif // SDATABASERECORD_H
