#include "susermodel.h"
#include <SLogRecordModel>
#include <ProjectQueries>
#include <SClientModel>

SUserModel::SUserModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "users";
    i_obligatoryFields << "username" << "name" << "office" << "photo" << "fields_cfg" << "notes" << "signature";
    setPrimaryKeyIndex(0);
    i_logRecord->set_type(SLogRecordModel::User);
}

SUserModel::~SUserModel()
{
    if(m_clientModel)
        delete m_clientModel;
}

QString SUserModel::constructSelectQuery()
{
    return QUERY_SEL_USER_DATA2(id());
}

void SUserModel::load()
{
    SSingleRowJModel::load();

    if(auto cid = clientUserId())
        m_clientModel = new SClientModel(cid, this);
    else
        m_clientModel = nullptr;

    loadSalaryRate();

    emit sigModelReset();
}

void SUserModel::overrideSalaryRate(const int rate)
{
    setData(C_salaryRate, rate);
    this->cacheRef(C_salaryRate)->setState(SSingleRowModelBase::ModifiedField::Executed);
}

void SUserModel::load(const int id)
{
    if(!id)
        return;

    setPrimaryKey(id);
    load();
}

void SUserModel::setSalaryRateStartDate(const QDate date)
{
    m_salaryRateStartDate = date;
}

void SUserModel::loadSalaryRate()
{
    overrideSalaryRate(0);

    if(m_salaryRateStartDate.isNull())
        return;

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("SELECT * FROM `salary_rates` WHERE `user_id` = %1 AND `start_from` <= '%2' ORDER BY `created_at` DESC LIMIT 1;").arg(id()).arg(m_salaryRateStartDate.toString("yyyy-MM-dd")));
    if(!record->first())
        return;

    overrideSalaryRate(record->value("value").toInt());
    m_salaryRateStartDate = QDate();

    delete record;
}

double SUserModel::balance()
{
    if(!m_clientModel)
        return 0;

    return m_clientModel->balance();
}

SClientModel *SUserModel::clientModel()
{
    return m_clientModel;
}

void SUserModel::setClientModel(SClientModel *model)
{
    m_clientModel = model;
}

void SUserModel::updateLogAssociatedRecId()
{
    i_logRecord->set_client(id());
}

bool SUserModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_created, QDateTime::currentDateTime());
    }

    return SSingleRowJModel::commit();
}

QString SUserModel::fullLongName()
{
    QString ret;
    ret = surname();
    if(!name().isEmpty())
        ret.append(" " + name());
    if(!patronymic().isEmpty())
        ret.append(" " + patronymic());

    return ret;
}

QString SUserModel::fullShortName()
{
    QString ret;
    ret = surname();
    if(!name().isEmpty())
        ret.append(" ").append(name().front()).append(".");
    if(!patronymic().isEmpty())
        ret.append(" ").append(patronymic().front()).append(".");

    return ret;
}
