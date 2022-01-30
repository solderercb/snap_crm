#ifndef WINDOWSDISPATCHER_H
#define WINDOWSDISPATCHER_H
#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>

class windowsDispatcher : public QObject
{
    Q_OBJECT
public:
    windowsDispatcher(QObject*);
    virtual ~windowsDispatcher(){};
public slots:
    void connectOK();
};

#endif
