#ifndef QUERYLOG_H
#define QUERYLOG_H

#include <QSqlQuery>
#include <QSqlRecord>
#include <QByteArray>
#include <QFile>
#include <QVariant>
#include <QDebug>

class queryLog
{
public:
    queryLog(QSqlQuery*);
    void setFile(const QString&);
    bool truncateLog();
    bool saveLog();

private:
    QSqlQuery *query;
    QFile file;
    QString generalLog;
    QString logOutput;
};

#endif // QUERYLOG_H
