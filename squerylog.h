#ifndef QUERYLOG_H
#define QUERYLOG_H

#include <QApplication>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QByteArray>
#include <QFile>
#include <QVariant>
#include <QDebug>

class SQueryLog
{
public:
    SQueryLog(QSqlQuery*);
    SQueryLog();
    explicit SQueryLog(QSqlDatabase db);
    ~SQueryLog();
    void setFile(const QString&);
    bool truncateLog();
    bool saveLog();
    void start(const QString &className);
    static SQueryLog* start(QSqlDatabase db, const QString &className);
    void stop();
private:
    QSqlQuery *query;
    QFile file;
    QString generalLog;
    QString logOutput;
    bool super_priv;
    bool loggingActive = 0;
};

#endif // QUERYLOG_H
