#include "slogrecordmodel.h"
#include <SUserSettings>
#include <ProjectGlobals>
#include <ProjectQueries>

SLogRecordModel::SLogRecordModel(QObject *parent) :
    SSingleRowModel(parent)
{
    mapFields();

    i_obligatoryFields << "user" << "created" << "values" << "values_after" << "office" << "notes";
    i_tableName = "logs";
    setPrimaryKeyIndex(0);
    setQueryIdBeforeInsert(false);
}

bool SLogRecordModel::commit()
{
    initMandatoryField(C_user, userDbData->id());
    initMandatoryField(C_values, QVariant());
    initMandatoryField(C_valuesAfter, QVariant());
    initMandatoryField(C_office, userDbData->currentOffice());
    initMandatoryField(C_created, QDateTime::currentDateTime());

#ifdef QT_DEBUG
    checkTableName();
#endif
    insert();
    setFieldsExecuted();

    return 1;
}

bool SLogRecordModel::commit(const QString &text)
{
    set_text(text);
    return commit();
}

void SLogRecordModel::unsetClient()
{
    setData(C_client, QVariant());
}

/* Переопределённый метод
 * Требуется пометка только поля id. Остальные поля остаются в статусе Updated, чтобы корректно работала запись нескольких сообщений в одной сесии.
 * В тестах это позволит проверить последнюю запись на предмет правильно отработавших алгоритмов коммита
*/
void SLogRecordModel::setAllState(ModifiedField::State state)
{
    if(!m_queryIdBeforeInsert)
        return;

    CacheMap::iterator i = cache.find(C_id);
    if(i != cache.end())
    {
        setState(*i, state);
    }
}
