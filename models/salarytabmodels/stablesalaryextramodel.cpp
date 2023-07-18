#include "global.h"
#include "stablesalaryextramodel.h"
#include "../../com_sql_queries.h"

STableSalaryExtraModel::STableSalaryExtraModel(QObject *parent) : SEditableBaseModel(parent, QSqlDatabase::database("connThird"))
{
    setEditStrategy(OnManualSubmit);
    setTable("additional_payments");
    connect(this, &STableSalaryExtraModel::primeInsert, this, &STableSalaryExtraModel::slotPrimeInsert);
}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryExtraModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(item.column())
        {
            case 2: return dataLocalizedFromDouble(item);
            case 3: return timestampLocal(item);
            case 4: return userFromId(item);
            default: ;
        }
    }
//    if(role == Qt::BackgroundColorRole)
//    {
//    }

    return SEditableBaseModel::data(item, role);
}

Qt::ItemFlags STableSalaryExtraModel::flags(const QModelIndex &item) const
{
    if(SEditableBaseModel::data(index(item.row(), 0)).toInt())
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    switch(item.column())
    {
        case 1:
        case 2: return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        default: ;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void STableSalaryExtraModel::setEmployeeId(const int id)
{
    m_employee = id;
}

void STableSalaryExtraModel::addNewRow()
{
    insertRow(rowCount());
}

void STableSalaryExtraModel::saveTable()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    m_queryLog = new SQueryLog();

    try
    {
        m_queryLog->start(metaObject()->className());
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);

        for(int i = 0; i < rowCount(); i++)
        {
            if(index(i, 0).data().toInt())
                continue;

            additionalPayment = new SAdditionalPaymentModel();

            additionalPayment->setName(SEditableBaseModel::data(index(i, 1)).toString());
            additionalPayment->setSumm(SEditableBaseModel::data(index(i, 2)).toDouble());
            additionalPayment->setPaymentDate(SEditableBaseModel::data(index(i, 3)).toDateTime());
            additionalPayment->setUser(userDbData->id);
            additionalPayment->setEmployee(m_employee);
            nErr = additionalPayment->commit();
            delete additionalPayment;

            // TODO Запись в журнал?

            if(!nErr)
                throw Global::ThrowType::QueryError;
        }
#ifdef QT_DEBUG
//        throw Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        // TODO всплывающее сообщение
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
//            nErr = 1; // это чтобы проверить работу дальше
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    m_queryLog->stop();
    if(nErr)
        emit repopulate();


    delete query;
    delete m_queryLog;
}

void STableSalaryExtraModel::slotPrimeInsert(int row, QSqlRecord &record)
{
    QSqlRecord *rec = new QSqlRecord();
    QSqlField *field;

    field = new QSqlField("id", QVariant::Int);
    rec->append(*field);
    delete field;

    field = new QSqlField("name", QVariant::String);
    rec->append(*field);
    delete field;

    field = new QSqlField("price", QVariant::Double);
    rec->append(*field);
    delete field;

    field = new QSqlField("payment_date"/*, QVariant::DateTime*/);
    field->setValue(QDateTime::currentDateTimeUtc());
    rec->append(*field);
    delete field;

    field = new QSqlField("user", QVariant::Int);
    field->setValue(userDbData->id);
    rec->append(*field);
    delete field;

    field = new QSqlField("to_user", QVariant::Int);
    field->setValue(m_employee);
    rec->append(*field);
    delete field;

    record = *rec;
}
