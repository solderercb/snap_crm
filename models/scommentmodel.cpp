#include "scommentmodel.h"

SCommentModel::SCommentModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "text" << "user";
    i_tableName = "comments";
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

QDateTime SCommentModel::created()
{
    return m_created;
}

void SCommentModel::setCreated(QDateTime created)
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

void SCommentModel::setRepair(int repairId)
{
    i_valuesMap.insert("remont", repairId);
    i_logRecord->setType(SLogRecordModel::Repair);
    i_logRecord->setRepairId(repairId);
    i_logRecord->setText(tr("Добавлен внутренний комментарий к ремонту №%1").arg(repairId));
}

int SCommentModel::client()
{
    return m_client;
}

void SCommentModel::setClient(int clientId)
{
    // В АСЦ v3.7.31.1123 не реализовано
    i_valuesMap.insert("client", clientId);
    i_logRecord->setType(SLogRecordModel::Client);
    i_logRecord->setClient(clientId);
    i_logRecord->setText(tr("Добавлен комментарий к карте клиента №%1").arg(clientId));
}

int SCommentModel::taskId()
{
    return m_taskId;
}

void SCommentModel::setTaskId(int taskId)
{
    i_valuesMap.insert("task_id", taskId);
    // TODO: В АСЦ v3.7.31.1123 не реализовано журналирование этой операции
//    i_logRecord->setType(SLogRecordModel::Client);
//    i_logRecord->setTaskId(taskId);
//    i_logRecord->setText(tr("Добавлен комментарий к заданию №%1").arg(taskId));
}

int SCommentModel::partRequest()
{
    return m_partRequest;
}

void SCommentModel::setPartRequest(int partRequestId)
{
    i_valuesMap.insert("part_request", partRequestId);
    i_logRecord->setType(SLogRecordModel::PartRequest);
    i_logRecord->setPartRequestId(partRequestId);
    i_logRecord->setText(tr("Добавлен комментарий к заявке на закупку №%1").arg(partRequestId));
}

bool SCommentModel::commit()
{
    if(i_id)
        update();
    else
    {
        setUser(userDbData->value("id").toInt());
        setCreated(QDateTime::currentDateTime());
        insert();
    }
    i_logRecord->commit();
    return i_nErr;
}

