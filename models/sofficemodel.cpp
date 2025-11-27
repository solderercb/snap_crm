#include "sofficemodel.h"
#include <ProjectGlobals>
#include <SUserSettings>
#include <SSqlQueryModel>

SOfficeModel* SOfficeModel::p_current = nullptr;


SOfficeModel::SOfficeModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "offices";
    i_obligatoryFields << "name" << "logo" << "administrator" << "default_company";
    setPrimaryKeyIndex(0);
}

SOfficeModel::~SOfficeModel()
{
    p_current = nullptr;
}

SOfficeModel *SOfficeModel::current(QObject *parent)
{
    if( !p_current )
        p_current = new SOfficeModel(parent);

    if(p_current)
        if( userDbData->currentOffice() > 0 && p_current->id() != userDbData->currentOffice() )
            p_current->load(userDbData->currentOffice());

    return p_current;
}

void SOfficeModel::load(const int &id)
{
    setPrimaryKey(id);

    SSingleRowJModel::load();
}

int SOfficeModel::defaultCompanyIndex()
{
    return companiesModel->rowByDatabaseID(defaultCompany());
}
