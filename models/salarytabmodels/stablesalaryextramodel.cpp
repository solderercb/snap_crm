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
            case Columns::Price: return dataLocalizedFromDouble(item);
            case Columns::PaymentDate: return timestampLocal(item);
            case Columns::User: return userFromId(item);
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
    if(SEditableBaseModel::data(item.siblingAtColumn(Columns::Id)).toInt())
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    switch(item.column())
    {
        case Columns::Name:
        case Columns::Price: return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
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
    if(rowCount() == 0)
        return;

    bool nErr = 1;
    QSqlQuery query(QSqlDatabase::database("connThird"));
    m_queryLog = new SQueryLog();

    m_queryLog->start(metaObject()->className());
    // TODO: все второстепенные операции (например, запуск журналирования, открытие сессии) нужно перенести в класс виджета tabSalary или SPageSalaryExtra для дальнейшей унификации кода
    try
    {
        SDatabaseRecord::checkSystemTime();

        QUERY_EXEC_TH(&query,nErr,QUERY_BEGIN);

        for(int i = 0; i < rowCount(); i++)
        {
            if(index(i, Columns::Id).data().toInt())
                continue;

            additionalPayment = new SAdditionalPaymentModel();

            additionalPayment->setName(SEditableBaseModel::data(index(i, Columns::Name)).toString());
            additionalPayment->setSumm(SEditableBaseModel::data(index(i, Columns::Price)).toDouble());
            additionalPayment->setPaymentDate(SEditableBaseModel::data(index(i, Columns::PaymentDate)).toDateTime());
            additionalPayment->setUser(userDbData->id);
            additionalPayment->setEmployee(m_employee);
            nErr = additionalPayment->commit();
            delete additionalPayment;

            // TODO Запись в журнал?

            if(!nErr)
                throw Global::ThrowType::QueryError;
        }
#ifdef QT_DEBUG
//        Global::throwDebug();
#endif
        QUERY_COMMIT_ROLLBACK(&query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if (type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(&query, nErr);
        }

//        if(type == Global::ThrowType::Debug)
//            nErr = 1; // это чтобы проверить работу дальше
    }

    m_queryLog->stop();
    if(nErr)
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Начисления и списания"), tr("Список успешно сохранён"), QColor(214,239,220), QColor(229,245,234));
        emit repopulate();
    }

    delete m_queryLog;
}

void STableSalaryExtraModel::slotPrimeInsert(int row, QSqlRecord &record)
{
    Q_UNUSED(row);
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
