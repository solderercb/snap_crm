#include "scommentmodel.h"

SCommentModel::SCommentModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "name" << "notes";
    tableName = "clients";

}

SCommentModel::~SCommentModel()
{

}

int SCommentModel::id()
{
    return m_id;
}

void SCommentModel::setId(int id)
{
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

QDate SCommentModel::created()
{
    return m_created;
}

void SCommentModel::setCreated(QDate created)
{
    i_valuesMap.insert("created", created);
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

void SCommentModel::setRemont(int remont)
{
    i_valuesMap.insert("remont", remont);
}

int SCommentModel::client()
{
    return m_client;
}

void SCommentModel::setClient(int client)
{
    i_valuesMap.insert("client", client);
}

int SCommentModel::taskId()
{
    return m_taskId;
}

void SCommentModel::setTaskId(int task_id)
{
    i_valuesMap.insert("task_id", task_id);
}

int SCommentModel::partRequest()
{
    return m_partRequest;
}

void SCommentModel::setPartRequest(int part_request)
{
    i_valuesMap.insert("part_request", part_request);
}

