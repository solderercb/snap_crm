#include "spaymenttypesmodel.h"
#include <SCashRegisterModel>
#include <ProjectQueries>

SPaymentTypesModel::SPaymentTypesModel(const int type, QObject *parent) : SStandardItemModel(parent)
{
    QVector<QString> paymentTypesList;
    QVector<int> paymentTypeIdsList;
    QList<QStandardItem*> *paymentTypeItem;

    if(type == Type::PKO)
    {
        // TODO: потенциальные проблемы!!! Описание см. в tabCashOperation::orderTypeChanged(int)
        paymentTypesList << tr("Приходный кассовый ордер без привязки") << tr("Предоплата за стоимость ремонта или деталей") << tr("Поступление средств на баланс клиента") << tr("Поступление денег в счет продажи товаров (РН)") << tr("Оплата ремонта") << tr("Оплата по счету");
        paymentTypeIdsList << SCashRegisterModel::RecptSimple << SCashRegisterModel::RecptPrepayRepair << SCashRegisterModel::RecptBalance << SCashRegisterModel::RecptGoods << SCashRegisterModel::RecptRepair << SCashRegisterModel::RecptInvoice;
    }
    else
    {
        paymentTypesList << tr("Расходный кассовый ордер без привязки") << tr("Оплата приходной накладной") << tr("Списание средств с баланса клиента") << tr("Z отчёт (выемка средств из кассы)") << tr("Возвратный РКО");
        paymentTypeIdsList << SCashRegisterModel::ExpSimple << SCashRegisterModel::ExpInvoice << SCashRegisterModel::ExpBalance << SCashRegisterModel::ExpZ << SCashRegisterModel::ExpRevert;
    }

    for (int i=0; i<paymentTypesList.size(); i++)
    {
        paymentTypeItem = new QList<QStandardItem*>();
        *paymentTypeItem << new QStandardItem(paymentTypesList.at(i)) << new QStandardItem(QString::number(paymentTypeIdsList.at(i)));
        appendRow(*paymentTypeItem);
    }
    setHorizontalHeaderLabels({"name", "id"});
    loadExtraTypes(type, false);
}

void SPaymentTypesModel::loadExtraTypes(const int type, const bool loadArchive)
{
    QList<QStandardItem*> *paymentTypeItem;
    QSqlQuery query = QSqlQuery(QSqlDatabase::database(TdConn::main()));
    QSqlRecord *record;

    query.exec(QUERY_SEL_EXTRA_PAYMENT_TYPES(type, loadArchive));
    while(query.next())
    {
        paymentTypeItem = new QList<QStandardItem*>();
        record = new QSqlRecord(query.record());
        *paymentTypeItem << new QStandardItem(record->value(0).toString()); // "name"
        for(int i = 2; i < record->count(); i++)
        {
            paymentTypeItem->first()->setData(record->value(i), Qt::UserRole + i);  // другие данные шаблона прячем в UserRole
        }
        // TODO: В АСЦ v3.7.31.1123 есть шаблоны только РКО
        *paymentTypeItem << new QStandardItem( QString::number(record->value(1).toInt() + SCashRegisterModel::ExpCustom) );    // "id"
        appendRow(*paymentTypeItem);
        delete record;
    }
}

QVariant SPaymentTypesModel::dataByDatabaseId(int id, int role)
{
    return index(rowByDatabaseID(id), 0).data(role);
}

/*  Возвращает текст основания ордера по id операции.
 *  Предназначена только для шаблонов кассовых ордеров
*/
QString SPaymentTypesModel::reasonByDatabaseId(int id)
{
    return dataByDatabaseId(id, Qt::UserRole + 7).toString();
}

int SPaymentTypesModel::clientByDatabaseId(int id)
{
    return dataByDatabaseId(id, Qt::UserRole + 3).toInt();
}

double SPaymentTypesModel::defaultAmountByDatabaseId(int id)
{
    return dataByDatabaseId(id, Qt::UserRole + 6).toDouble();
}

int SPaymentTypesModel::paymentSystemByDatabaseId(int id)
{
    return dataByDatabaseId(id, Qt::UserRole + 10).toInt();
}

