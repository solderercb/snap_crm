#ifndef SCOMSETTINGSTYPES_H
#define SCOMSETTINGSTYPES_H
#include <3rdparty/QSerializer/QSerializer>

class t_status : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_FIELD(int, Id)
    QS_JSON_FIELD(QString, Name)
    QS_JSON_COLLECTION(QList, int, Contains)
    QS_JSON_COLLECTION(QList, int, Actions)
    QS_JSON_COLLECTION(QList, int, Roles)
    QS_JSON_FIELD(QString, Color)
    QS_JSON_FIELD(QString, Terms)
};

class t_repairStatuses : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_COLLECTION_OBJECTS(QVector, t_status, status)
};

class t_emailConfig : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_FIELD(QString, Host)
    QS_JSON_FIELD(int, Port)
    QS_JSON_FIELD(QString, Login)
    QS_JSON_FIELD(QString, Password)
    QS_JSON_FIELD(int, Timeout)
    QS_JSON_FIELD(bool, EnableSsl)
    QS_JSON_FIELD(bool, EnableImplicitSsl)
    QS_JSON_FIELD(QString, Template)
};

class t_repairDispatcherUsers : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_COLLECTION(QList, QString, Users)
};

class t_repairDispatcherAssignCriteria : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_COLLECTION(QList, QString, StatusList)
    QS_JSON_FIELD(int, DayLimit)
};

class t_smsConfig : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_FIELD(int, Provider)
    QS_JSON_FIELD(int, AuthType)
    QS_JSON_FIELD(QString, ApiId)
    QS_JSON_FIELD(QString, Login)
    QS_JSON_FIELD(QString, Password)
    QS_JSON_FIELD(QString, Sender)
};

#endif // SCOMSETTINGSTYPES_H
