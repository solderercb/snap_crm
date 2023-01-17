#ifndef SCOMMENTMODEL_H
#define SCOMMENTMODEL_H

#include "scomrecord.h"
#include <QObject>
#include <QDateTime>
#include <QString>

class SCommentModel : public SComRecord
{
    Q_OBJECT
public:
    enum Columns {ColId = 0, ColCreated, ColUser, ColText, ColRepair, ColClient, ColTask, ColPartRequest};
    enum Mode {NotSet = 0, Repair = 1, Client, Task, PartRequest};
    explicit SCommentModel(QObject *parent = nullptr);
    explicit SCommentModel(QList<QStandardItem*> &record, QObject *parent = nullptr);
    ~SCommentModel();
    int id();
    void setId(const int);
    QString text();
    void setText(const QString);
    QDateTime created();
    void setCreated(const QDateTime);
    int user();
    void setUser(const int);
    int remont();
    void setRepair(const int);
    int client();
    void setClient(const int);
    int taskId();
    void setTask(const int);
    int partRequest();
    void setPartRequest(const int);
    bool commit();
    void setObjId(const int mode, const int id);
    void setQueryField(const int fieldNum, const QVariant value, const QVariant oldValue) override;
    bool remove();
private:
    int m_id;
    QString m_text;
    QDateTime m_created;
    int m_user;
    int m_remont;
    int m_client;
    int m_taskId;
    int m_partRequest;
};

#endif // SCOMMENTMODEL_H
