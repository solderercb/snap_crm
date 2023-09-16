#ifndef SCOMSETTINGSTYPES_H
#define SCOMSETTINGSTYPES_H
#include <3rdparty/QSerializer/QSerializer>

class t_status : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_FIELD(int, Id)
    QS_FIELD(QString, Name)
    QS_COLLECTION(QList, int, Contains)
    QS_COLLECTION(QList, int, Actions)
    QS_COLLECTION(QList, int, Roles)
    QS_FIELD(QString, Color)
    QS_FIELD(QString, Terms)
};

class t_repairStatuses : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_COLLECTION_OBJECTS(QVector, t_status, status)
};

class t_emailConfig : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_FIELD(QString, Host)
    QS_FIELD(int, Port)
    QS_FIELD(QString, Login)
    QS_FIELD(QString, Password)
    QS_FIELD(int, Timeout)
    QS_FIELD(bool, EnableSsl)
    QS_FIELD(bool, EnableImplicitSsl)
    QS_FIELD(QString, Template)
};

class t_repairDispatcherUsers : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_COLLECTION(QList, QString, Users)
};

class t_repairDispatcherAssignCriteria : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_COLLECTION(QList, QString, StatusList)
    QS_FIELD(int, DayLimit)
};

class t_smsConfig : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_FIELD(int, Provider)
    QS_FIELD(int, AuthType)
    QS_FIELD(QString, ApiId)
    QS_FIELD(QString, Login)
    QS_FIELD(QString, Password)
    QS_FIELD(QString, Sender)
};

#endif // SCOMSETTINGSTYPES_H
