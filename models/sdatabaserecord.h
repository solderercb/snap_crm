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
#include "models/sdatabaseauxiliary.h"

class SDatabaseRecord : public SSqlQueryModel, public SDatabaseAuxiliary
{
    Q_OBJECT
    friend class SComRecord;
signals:
    void modelUpdated();
public:
    explicit SDatabaseRecord(QObject *parent = nullptr);
    ~SDatabaseRecord();
    int lastInsertId();
    virtual QDateTime createdUtc();
    QString created();
    void setCreated(const QDateTime &timestamp);
    static bool checkSystemTime();
    bool isDirty();
    virtual void setDirty(bool state);
    QSqlError lastError();
protected:
    int i_id = 0;
    bool i_nErr = 1;
    QString i_tableName;
    QStringList i_obligatoryFields = {};   // поля не имеющие значения по умолчанию
    QDateTime i_createdUtc;
    QSqlQuery *i_query;
    virtual void queryNewId(int &newId);
    virtual void queryLastInsertId();
    bool insert(bool flush = true);
    bool update();
    bool del();
    QString i_idColumnName;
    virtual bool commit();
    virtual void dbErrFlagHandler(bool);
private:
    bool m_isIdColumnNameSet = 0;
    bool m_isDirty = 0;
    bool checkObligatoryFields();
    bool checkTableName();
signals:

};

#endif // SDATABASERECORD_H
