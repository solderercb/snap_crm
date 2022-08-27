#ifndef SCOMMENTMODEL_H
#define SCOMMENTMODEL_H

#include "scomrecord.h"
#include <QObject>

class SCommentModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SCommentModel(QObject *parent = nullptr);
    ~SCommentModel();
    int id();
    void setId(const int);
    QString text();
    void setText(const QString);
    QDate created();
    void setCreated(const QDate);
    int user();
    void setUser(const int);
    int remont();
    void setRemont(const int);
    int client();
    void setClient(const int);
    int taskId();
    void setTaskId(const int);
    int partRequest();
    void setPartRequest(const int);
private:
    int m_id;
    QString m_text;
    QDate m_created;
    int m_user;
    int m_remont;
    int m_client;
    int m_taskId;
    int m_partRequest;
};

#endif // SCOMMENTMODEL_H
