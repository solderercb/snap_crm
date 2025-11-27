#include "sstoresaleitemmodel.h"
#include <ProjectGlobals>
#include <ssaletablemodel>
#include <SUserSettings>
#include <SStoreItemModel>
#include <SLogRecordModel>

SStoreSaleItemModel::SStoreSaleItemModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "store_sales";
    i_obligatoryFields << "dealer" << "item_id" << "document_id" << "user";
    setPrimaryKeyIndex(0);
    connect(this, &SSingleRowModel::beginDataChange, this, &SStoreSaleItemModel::setDataRework);

    m_storeItem = std::make_shared<SStoreItemModel>();
    m_storeItem->setSaleMode(SStoreItemModel::SaleMode::Store);
}


SStoreSaleItemModel::~SStoreSaleItemModel()
{
}

/*
 * Возвращает 0 в случае ошибки
 */
bool SStoreSaleItemModel::sale()
{
    bool nErr = 1;
    QString logText;
    if(isPrimaryKeyValid())
    {
        int savedCount = commitedData(C_count).value_or(0).toInt();
        if(count() < savedCount)
            logText = tr("Продажа ранее зарезервированного товара: %1ед.; отмена резерва (невостребовано): %2ед.").arg(count()).arg(savedCount - count());
        else if(count() > savedCount)
            logText = tr("Продажа ранее зарезервированного товара: %1ед.; дополнительно: %2ед.").arg(savedCount).arg(count() - savedCount);
        else
            logText = tr("Продажа ранее зарезервированного товара: %1ед.").arg(count());
        nErr &= m_storeItem->saleStore(logText);
    }
    else
    {
        logText = tr("Продажа товара в кол-ве %1ед.").arg(count());
        nErr &= m_storeItem->sale(logText);
    }
    commit();
    return nErr;
}

bool SStoreSaleItemModel::reserve()
{
    bool nErr = 1;
    commit();
    nErr &= m_storeItem->reserve(tr("Резерв %1ед. товара").arg(count()));
    return nErr;
}

bool SStoreSaleItemModel::unsale()
{
    bool nErr = 1;
    QString logText = tr("Возврат %1ед. товара").arg(commitedData(C_count)->toInt());

    if(!unsaleReason().isEmpty())
        logText.append(tr(", причина: %1", "причина возврата, указанная пользователем").arg(unsaleReason()));
    set_state(State::Cancelled);
    initMandatoryField(C_unsaleReason, "unsale");
    commit();
    nErr &= m_storeItem->unsale(logText);
    return nErr;
}

bool SStoreSaleItemModel::free()
{
    bool nErr = 1;
    QString logText = tr("Отмена резерва %1ед. товара").arg(commitedData(C_count)->toInt());
    set_state(State::Cancelled);
    set_unsaleReason("cancel reserve");
    nErr &= m_storeItem->free(logText);
    commit();
    return nErr;
}

bool SStoreSaleItemModel::isProfitable()
{
    // TODO: проверка указанной цены; выдача предупреждения при продаже в минус
    return 1;
}

// продажа ранее зарезервированного товара, возврат или отмена резерва
bool SStoreSaleItemModel::commit()
{
    if(!isPrimaryKeyValid())
    {
    }

    // при пометке строки на удаление в StoreSaleModel::markRowRemove изменяется только статус;
    // кол-во обнуляем только перед коммитом
    if(data(C_state, ModifiedField::Updated).toInt() == State::Cancelled)
        set_count(0);

    return SSingleRowJModel::commit();
}

void SStoreSaleItemModel::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_buyer: if(!data.toInt()) data = QVariant(); break;
        default: ;
    }
}

void SStoreSaleItemModel::logDataChange(const int index, const QVariant &value)
{
    switch (index)
    {
        case C_count: m_storeItem->setNewExpense(value.toInt()); break;
        case C_documentId: i_logRecord->set_document(value.toInt()); break;
        default: break;
    }
}

/* Переопределённый метод.
 * Дополнительно к базовому функционалу инициализирует поле SStoreItemModel
*/
bool SStoreSaleItemModel::setData(const int index, const QVariant &value)
{
    if(index < 0)
        return 1;

    if(index == Columns::C_itemId)
        m_storeItem->setPrimaryKey(value);

    return SSingleRowJModel::setData(index, value);
}

void SStoreSaleItemModel::initFieldWithPrevLoaded(const int index, const QVariant &value)
{
    SSingleRowJModel::initFieldWithPrevLoaded(index, value);

    switch (index)
    {
        case C_documentId: i_logRecord->set_document(value.toInt()); break;
        default: break;
    }
}
