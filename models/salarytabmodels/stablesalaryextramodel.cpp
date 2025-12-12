#include "stablesalaryextramodel.h"
#include <ProjectGlobals>
#include <SSalaryAdditionalPaymentModel>
#include <SUserSettings>
#include <QSqlField>

STableSalaryExtraModel::STableSalaryExtraModel(QObject *parent) : SEditableBaseModel(parent, QSqlDatabase::database(TdConn::session()))
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
    for(int i = 0; i < rowCount(); i++)
    {
        if(index(i, Columns::Id).data().toInt())
            continue;

        auto additionalPayment = std::make_unique<SAdditionalPaymentModel>();

        additionalPayment->set_name(SEditableBaseModel::data(index(i, Columns::Name)).toString());
        additionalPayment->set_summ(SEditableBaseModel::data(index(i, Columns::Price)).toDouble());
        additionalPayment->set_paymentDate(SEditableBaseModel::data(index(i, Columns::PaymentDate)).toDateTime());
        additionalPayment->set_user(userDbData->id());
        additionalPayment->set_employee(m_employee);
        additionalPayment->commit();

        // TODO Запись в журнал?
    }
#ifdef QT_DEBUG
//        Global::throwDebug();
#endif
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
    field->setValue(userDbData->id());
    rec->append(*field);
    delete field;

    field = new QSqlField("to_user", QVariant::Int);
    field->setValue(m_employee);
    rec->append(*field);
    delete field;

    record = *rec;
}
