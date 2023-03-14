#include "scommentmodel.h"

SCommentModel::SCommentModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "text" << "user";
    i_tableName = "comments";
}

SCommentModel::SCommentModel(QList<QStandardItem *> &record, QObject *parent) :
    SCommentModel(parent)
{
    i_id = record.at(SCommentModel::ColId)->data(Qt::DisplayRole).toInt();

    i_createdUtc = record.at(SCommentModel::ColCreated)->data(Qt::DisplayRole).toDateTime();
    m_user = record.at(SCommentModel::ColUser)->data(Qt::DisplayRole).toInt();
    m_text = record.at(SCommentModel::ColText)->data(Qt::DisplayRole).toString();

    initQueryFields(record);
}

SCommentModel::~SCommentModel()
{
}

int SCommentModel::id()
{
    return i_id;
}

void SCommentModel::setId(int id)
{
    i_id = id;
    i_valuesMap.insert("id", id);
}

QString SCommentModel::text()
{
    return m_text;
}

void SCommentModel::setText(QString text)
{
    i_valuesMap.insert("text", text);
}

int SCommentModel::user()
{
    return m_user;
}

void SCommentModel::setUser(int user)
{
    i_valuesMap.insert("user", user);
}

int SCommentModel::remont()
{
    return m_remont;
}

void SCommentModel::setRepair(int id)
{
    if(!i_id)
    {
        i_valuesMap.insert("remont", id);
    }
}

int SCommentModel::client()
{
    return m_client;
}

void SCommentModel::setClient(int clientId)
{
    // В АСЦ v3.7.31.1123 не реализовано
    if(!i_id)
    {
        i_valuesMap.insert("client", clientId);
    }
}

int SCommentModel::taskId()
{
    return m_taskId;
}

void SCommentModel::setTask(int taskId)
{
    if(!i_id)
    {
        i_valuesMap.insert("task_id", taskId);
    }
}

int SCommentModel::partRequest()
{
    return m_partRequest;
}

void SCommentModel::setPartRequest(int partRequestId)
{
    if(!i_id)
    {
        i_valuesMap.insert("part_request", partRequestId);
    }
}

bool SCommentModel::commit()
{
    if(i_id)
        update();
    else
    {
        if(!i_valuesMap.contains("user"))
            setUser(userDbData->value("id").toInt());
        if(!i_valuesMap.contains("created"))
            setCreated(QDateTime::currentDateTime());
        insert();
    }

    return i_nErr;
}

void SCommentModel::setObjId(const int mode, const int id)
{
    switch (mode)
    {
        case Mode::Repair: setRepair(id); break;
        case Mode::Client: setClient(id); break;
        case Mode::Task: setTask(id); break;
        case Mode::PartRequest: setPartRequest(id); break;
    }
}

void SCommentModel::setQueryField(const int fieldNum, const QVariant value, const QVariant)
{
    switch(fieldNum)
    {
        case SCommentModel::ColCreated: setCreated(value.toDateTime()); break;
        case SCommentModel::ColUser: setUser(value.toInt()); break;
        case SCommentModel::ColText: setText(value.toString()); break;
    }
}

bool SCommentModel::remove()
{
    if(!i_id)
        return 0;

    return del();
}

