#include "suseractivitymodel.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SUserSettings>
#include <QHostInfo>

SUserActivityModel::SUserActivityModel(QObject *parent) : SSingleRowModel(parent)
{
    mapFields();

    i_obligatoryFields << "user_id" << "datetime_" << "notes";
    i_tableName = "users_activity";
    setQueryIdBeforeInsert(false);
    setPrimaryKeyIndex(0);
}

SUserActivityModel::~SUserActivityModel()
{
}

void SUserActivityModel::appendRecord(const QString &notes)
{
    set_notes(notes);
    userDbData->updateActivityTimestamp();
    commit();
}

void SUserActivityModel::appendRecordStandalone(const QString &notes)
{
    try
    {
        initSqlQuery();
        QUERY_EXEC_TH(i_query,true,QUERY_BEGIN);
        appendRecord(notes);
        QUERY_COMMIT_ROLLBACK(i_query, true);
    }
    catch(Global::ThrowType type)
    {
        if (type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(i_query, true);
        }
    }
}

bool SUserActivityModel::commit()
{
    bool nErr = 1;
    set_userId(userDbData->id());
    set_datetime(QDateTime::currentDateTime());
    set_appVersion(APP_VER_STR);
    set_machineName(QHostInfo::localHostName());

#ifdef QT_DEBUG
    checkTableName();
#endif
    nErr = insert();
    setFieldsExecuted();

    return nErr;
}

/* Переопределённый метод
 * Пометка не требуется, поля остаются в статусе Updated, чтобы корректно работала запись нскольких сообщений в одной сесии.
*/
void SUserActivityModel::setAllState(ModifiedField::State)
{
}
