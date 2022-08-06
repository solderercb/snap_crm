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
    bool i_nDBErr = 1;
    QDateTime i_createdUtc;
    bool checkSystemTime();
    bool checkObligatoryFields();
    QMap<QString, QVariant> i_valuesMap;   // key - field name
    QStringList i_obligatoryFields = {};   // поля не имеющие значения по умолчанию
    QSqlQuery *i_query;
    QStringList fields, field_values;
    void fieldsInsFormatter();
    void fieldsUpdFormatter();
    QString fieldValueHandler(const QVariant&);
    bool insert(const QString&);
    bool update(const QString&, const int);
    bool del(const QString&, const int);
private:
signals:

};

#endif // SDATABASERECORD_H
