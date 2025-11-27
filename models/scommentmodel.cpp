#include "scommentmodel.h"
#include <SUserSettings>
#include <ProjectGlobals>

SCommentModel::SCommentModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "text" << "user";
    i_tableName = "comments";
    i_primaryKeyIndex = 0;
}

SCommentModel::~SCommentModel()
{
}

bool SCommentModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_user, userDbData->id());
        initMandatoryField(C_created, QDateTime::currentDateTimeUtc());
    }

    SSingleRowJModel::commit();

    return 1;
}

void SCommentModel::setObjId(const int mode, const int id)
{
    switch (mode)
    {
        case Mode::Repair: set_repair(id); break;
        case Mode::Client: set_client(id); break;
        case Mode::Task: set_task(id); break;
        case Mode::PartRequest: set_partRequest(id); break;
    }
}

bool SCommentModel::remove()
{
    if(!isPrimaryKeyValid())
        return 0;

    return del();
}

