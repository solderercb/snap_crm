#include "sstoresaleitemmodel.h"

SStoreSaleItemModel::SStoreSaleItemModel(const QList<QStandardItem *> &record, QObject *parent) : SComRecord(parent)
{
    i_tableName = "store_sales";
    i_obligatoryFields << "dealer" << "item_id" << "document_id" << "user";

    i_id = record.at(ColId)->data(Qt::DisplayRole).toInt();
    m_item_id = record.at(ColItemId)->data(Qt::DisplayRole).toInt();
    m_item_name = record.at(ColName)->data(Qt::DisplayRole).toString();
    m_doc_id = record.at(ColDocument)->data(Qt::DisplayRole).toInt();
    m_dealer = record.at(ColDealer)->data(Qt::DisplayRole).toInt();
    m_customer = record.at(ColCustomer)->data(Qt::DisplayRole).toInt();
    m_isRealization = record.at(ColRealization)->data(Qt::DisplayRole).toBool();
    m_returnPercent = record.at(ColRetPercent)->data(Qt::DisplayRole).toInt();
    m_inPrice = record.at(ColInPrice)->data(Qt::DisplayRole).toFloat();
    m_price = record.at(ColPrice)->data(Qt::DisplayRole).toFloat();
    m_count = record.at(ColCount)->data(Qt::DisplayRole).toInt();
    m_avail = record.at(ColAvail)->data(Qt::DisplayRole).toInt();
    m_old_count = count();

    // TODO: сделать выборочную передачу значений: для не новой РН нужно передавать только изменённые данные
//    if(!i_id)
//    {
        i_valuesMap.insert("user", userDbData->value("id"));
        i_valuesMap.insert("item_id", m_item_id);
        i_valuesMap.insert("count", m_count);
        i_valuesMap.insert("in_price", m_inPrice);
        i_valuesMap.insert("price", m_price);
        i_valuesMap.insert("dealer", m_dealer);
        i_valuesMap.insert("warranty", record.at(ColWarranty)->data(Qt::DisplayRole).toInt());
        i_valuesMap.insert("is_realization", m_isRealization);
        i_valuesMap.insert("is_cancellation", record.at(ColState)->data(Qt::DisplayRole).toBool());
//        i_valuesMap.insert("dealer_payment", record.at()->data(Qt::DisplayRole).toBool());
        i_valuesMap.insert("return_percent", m_returnPercent);
        i_valuesMap.insert("sn", record.at(ColSN)->data(Qt::DisplayRole).toString());
//    }
//    else
//    {
//        i_valuesMap.insert("count", m_count);
//    }

    i_logRecord->setType(SLogRecordModel::Part);
    i_logRecord->setItemId(m_item_id);
    if(m_doc_id)
        i_logRecord->setDocumentId(m_doc_id);
    if(m_customer)
        i_logRecord->setClient(m_customer);
}

SStoreSaleItemModel::~SStoreSaleItemModel()
{
}

void SStoreSaleItemModel::setClient(int id)
{
    if(id == 0)
    {
        unsetClient();
        return;
    }

    i_valuesMap.insert("customer_id", id);
    i_logRecord->setClient(id);
}

void SStoreSaleItemModel::unsetClient()
{
    i_valuesMap.insert("customer_id", QVariant());
    i_logRecord->unsetClient();
}

void SStoreSaleItemModel::setDocumentId(int id)
{
    m_doc_id = id;
    i_valuesMap.insert("document_id", id);
    i_logRecord->setDocumentId(id);
}

/*
 * Возвращает 0 в случае ошибки
 */
bool SStoreSaleItemModel::sale()
{
    QString text;
    if(i_id)
    {
        if(m_count < m_old_count)
            text = tr("Продажа ранее зарезервированного товара: %1ед.; отмена резерва (невостребовано): %2ед.").arg(m_count).arg(m_old_count - m_count);
        else if(m_count > m_old_count)
            text = tr("Продажа ранее зарезервированного товара: %1ед.; дополнительно: %2ед.").arg(m_old_count).arg(m_count - m_old_count);
        else
            text = tr("Продажа ранее зарезервированного товара: %1ед.").arg(m_count);
    }
    else
        text = tr("Продажа товара в кол-ве %1ед.").arg(m_count);

    i_logRecord->setText(text);
    commit(i_id?SaleReserved:Sale);
    return i_nErr;
}

bool SStoreSaleItemModel::reserve()
{
    i_logRecord->setText(tr("Резерв %1ед. товара").arg(m_count));
    commit(Reserve);
    return i_nErr;
}

bool SStoreSaleItemModel::unsale()
{
    QString logText = tr("Возврат %1ед. товара").arg(m_old_count);
    if(!m_unsaleReason.isEmpty())
        logText.append(tr(", причина: %1").arg(m_unsaleReason));
    i_logRecord->setText(logText);
    commit(Unsale);
    return i_nErr;
}

void SStoreSaleItemModel::setUnsaleReason(const QString &reason)
{
    m_unsaleReason = reason;
}

bool SStoreSaleItemModel::free()
{
    i_logRecord->setText(tr("Отмена резерва %1ед. товара").arg(m_old_count));
    commit(Free);
    return i_nErr;
}

/* Получение "свежих известий" о кол-ве товаров
 * Пока первый пользователь собирал заказ, товар мог быть продан другим пользователем
 */
QSqlRecord* SStoreSaleItemModel::actualStoreQtys()
{

    QSqlRecord *record = new QSqlRecord();

    QUERY_EXEC(i_query,i_nErr)(QUERY_SEL_ITEM_ACTUAL_QTY(m_item_id));
    if((i_nErr && i_query->first()))
        *record = i_query->record();

    return record;
}

bool SStoreSaleItemModel::isProfitable()
{
    // TODO: проверка указанной цены; выдача предупреждения при продаже в минус
    return 1;
}

bool SStoreSaleItemModel::integrityStatus()
{
    return nIntegrityErr;
}

/* Верификация записанных данных
 *
 */
bool SStoreSaleItemModel::verifyQty(QSqlRecord *before, QSqlRecord *after)
{
    if( count() != m_count )
        nIntegrityErr = 0;

    if(nIntegrityErr && m_op_type == Sale)
    {
        if(nIntegrityErr && before->value(0).toInt() - m_count != after->value(0).toInt())   // `count`
            nIntegrityErr = 0;
        if(nIntegrityErr && before->value(1).toInt() + m_count != after->value(1).toInt())   // `sold`
            nIntegrityErr = 0;
    }
    else if(nIntegrityErr && m_op_type == Reserve)
    {
        if(nIntegrityErr && before->value(2).toInt() + m_count != after->value(2).toInt())   // `reserved`
            nIntegrityErr = 0;
    }
    else if(nIntegrityErr && m_op_type == SaleReserved)
    {
        if(nIntegrityErr && before->value(0).toInt() - m_count != after->value(0).toInt())   // `count`
            nIntegrityErr = 0;
        if(nIntegrityErr && before->value(1).toInt() + m_count != after->value(1).toInt())   // `sold`
            nIntegrityErr = 0;

        if(nIntegrityErr && before->value(2).toInt() - m_old_count != after->value(2).toInt())   // `reserved`
            nIntegrityErr = 0;
    }
    else if(nIntegrityErr && m_op_type == Unsale)
    {
        if(nIntegrityErr && before->value(0).toInt() + m_old_count != after->value(0).toInt())   // `count`
            nIntegrityErr = 0;
        if(nIntegrityErr && before->value(1).toInt() - m_old_count != after->value(1).toInt())   // `sold`
            nIntegrityErr = 0;
    }
    else if(nIntegrityErr && m_op_type == Free)
    {
        if(nIntegrityErr && before->value(2).toInt() - m_old_count != after->value(2).toInt())   // `reserved`
            nIntegrityErr = 0;
    }

    return nIntegrityErr;
}

void SStoreSaleItemModel::showNotification(const QString &text)
{
    QMessageBox msgBox;

    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

/*  Возвращает значение `store_sales`.`count` из БД или 0 */
int SStoreSaleItemModel::count()
{
    if( i_id == 0 )
        return 0;

    QUERY_EXEC(i_query,i_nErr)( QUERY_SEL_STORE_SALES_QTY(i_id));
    i_query->first();
    return i_query->value(0).toInt();
}

bool SStoreSaleItemModel::dealerRoyalty()
{
    SClientModel *dealer = new SClientModel();
    dealer->setId(m_dealer);
    dealer->createBalanceObj();
    try
    {
        if(m_op_type == Unsale)
        {
            float dealerRoyalty = m_old_count*( m_inPrice + (m_price - m_inPrice)*m_returnPercent/100 );
            i_nErr = dealer->updateBalance(-dealerRoyalty, tr("Списание %1 по причение возврата %2ед. товара %3, находившегося на реализации").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_old_count).arg(m_item_id), SBalanceLogRecordModel::RoyaltyReason::Document, m_doc_id);
        }
        else
        {
            float dealerRoyalty = m_count*( m_inPrice + (m_price - m_inPrice)*m_returnPercent/100 );
            i_nErr = dealer->updateBalance(dealerRoyalty, tr("Зачисление %1 за %2ед. проданного товара %3, находившегося на реализации").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_count).arg(m_item_id), SBalanceLogRecordModel::RoyaltyReason::Document, m_doc_id);
        }
    }
    catch (int)
    {
        i_nErr = 0;
        showNotification(tr("Ошибка целостности данных баланса, dealer = %1").arg(m_dealer));
    }
    delete dealer;

    return i_nErr;
}

bool SStoreSaleItemModel::commit()
{
    QSqlRecord *before, *after;

    before = actualStoreQtys();
    if(checkAvailabilityBefore(before))
    {
        shortlivedNotification *newPopup = new shortlivedNotification(parent(), tr("Товар отсутствует"), tr("\"%1\" (UID %2) больше не доступен для продажи").arg(m_item_name).arg(m_item_id), QColor(255,164,119), QColor(255,199,173));
        i_nErr = 0;
    }
    else
    {
        if( (m_op_type == Sale) || (m_op_type == Reserve) )
        {
            insert();
        }
        else    // продажа ранее зарезервированного товара, возврат или отмена резерва
        {
            update();
        }

        i_logRecord->commit();

        if( m_isRealization && (m_op_type == Sale || m_op_type == SaleReserved || m_op_type == Unsale) )
        {
            dealerRoyalty();
        }

        updateStoreItemsTable();
        after = actualStoreQtys();
        // TODO: если последняя единица товара, нужно удалить ячейку и сделать соотв. запись в журнал (но! нужно чтобы при вызове updateWidgets() в таблице место не обновлялось)
        // а может и не нужно; в АСЦ удалялась, но за годы пользования я так и не понял зачем это сделано

        if(i_nErr && !verifyQty(before, after))
        {
            i_nErr = 0;
            showNotification(tr("Ошибка целостности данных item_id = %1").arg(m_item_id));
        }

        delete after;
    }
    delete before;

    return i_nErr;
}

bool SStoreSaleItemModel::commit(Operation op)
{
    m_op_type = op;
    return commit();
}

bool SStoreSaleItemModel::checkAvailabilityBefore(QSqlRecord *before)
{
    // В накладной резерва допускается изменение кол-ва (вдруг клиент решил купить меньше/больше);
    // в этом случае при проверке не будет учитываться старое кол-во
    if(m_count > (before->value(0).toInt() - before->value(2).toInt() + m_old_count) )
    {
        emit qtyNotEnough();
        return 1;
    }
    return 0;
}

bool SStoreSaleItemModel::updateStoreItemsTable()
{
    switch (m_op_type)
    {
        case Sale         : QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_STORE_ITEMS_SALE(m_count, m_item_id)); break;
        case Reserve      : QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_STORE_ITEMS_RESERVE(m_count, m_item_id)); break;
        case SaleReserved : QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_STORE_ITEMS_SALE_RESERVED(m_item_id, m_count, m_old_count)); break;
        case Unsale       : QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_STORE_ITEMS_UNSALE(m_item_id, m_old_count)); break;
        case Free         : QUERY_EXEC(i_query,i_nErr)(QUERY_UPD_STORE_ITEMS_RESERVE_CANCELLATION(m_item_id, m_old_count)); break;
        case Nop:
        case SaleRepair: break;
        default: break;
    }

    return i_nErr;
}
