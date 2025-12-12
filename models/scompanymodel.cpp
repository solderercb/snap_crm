#include "scompanymodel.h"
#include <ProjectGlobals>
#include <SUserSettings>
#include <SSqlQueryModel>
#include <SUserModel>

SCompanyModel* SCompanyModel::p_current = nullptr;


SCompanyModel::SCompanyModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "companies";
    i_obligatoryFields << "name";
    setPrimaryKeyIndex(0);
}

SCompanyModel::~SCompanyModel()
{
    p_current = nullptr;
}

SCompanyModel *SCompanyModel::current(QObject *parent)
{
    if( !p_current )
        p_current = new SCompanyModel(parent);

    if(p_current)
        if( userDbData->company() > 0 && p_current->id() != userDbData->company() )
            p_current->load(userDbData->company());

    return p_current;
}

void SCompanyModel::load(const int &id)
{
    setPrimaryKey(id);

    SSingleRowJModel::load();
}

void SCompanyModel::userNames(const int id, QString &surname, QString &name, QString &patronymic)
{
    int row = usersModel->rowByDatabaseID(id);
    surname = usersModel->record(row).value("surname").toString();
    name = usersModel->record(row).value("name").toString();
    patronymic = usersModel->record(row).value("patronymic").toString();
}

QVariant SCompanyModel::ceoLongName()
{
    QString surname, name, patronymic;
    userNames(ceo(), surname, name, patronymic);
    return SUserModel::fullLongName(surname, name, patronymic);
};

QVariant SCompanyModel::ceoShortName()
{
    QString surname, name, patronymic;
    userNames(ceo(), surname, name, patronymic);
    return SUserModel::fullShortName(surname, name, patronymic);
};

QVariant SCompanyModel::caoLongName()
{
    QString surname, name, patronymic;
    userNames(cao(), surname, name, patronymic);
    return SUserModel::fullLongName(surname, name, patronymic);
};

QVariant SCompanyModel::caoShortName()
{
    QString surname, name, patronymic;
    userNames(cao(), surname, name, patronymic);
    return SUserModel::fullShortName(surname, name, patronymic);
};
