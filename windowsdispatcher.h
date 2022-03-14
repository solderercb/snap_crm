#ifndef WINDOWSDISPATCHER_H
#define WINDOWSDISPATCHER_H
#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include "com_sql_queries.h"

class windowsDispatcher : public QObject
{
    Q_OBJECT
signals:
    void quit();
public:
    windowsDispatcher(QObject* parent = nullptr);
    virtual ~windowsDispatcher();
    QString test = "QString test";
    QSqlQuery *queryUserData;
    QSqlQuery* queryPermissions;
    QMap<QString, QVariant> *userData;
    QMap<QString, QVariant> *userLocalData;
    QMap<QString, bool> *permissions;
    QSqlQueryModel* companiesModel;
    QSqlQueryModel* officesModel;
public slots:
    void connectOK();
    void createChooseOfficeWindow();
    void createMainWindow();
};

#endif
