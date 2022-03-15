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
    QSqlQuery* queryPermissions;

public slots:
    void connectOK();
    void createChooseOfficeWindow();
    void createMainWindow();
};

#endif
