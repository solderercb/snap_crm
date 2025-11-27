#include "squerylog.h"
#include <SLocalSettings>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QByteArray>
#include <QVariant>
#include <QDebug>

SQueryLog::SQueryLog(QSqlQuery *q) :
    query(q)
{
    query->exec("SELECT IF(Super_priv LIKE 'Y', 1, 0) FROM mysql.user WHERE `User` = SUBSTRING_INDEX(USER(), '@', 1) AND SUBSTRING_INDEX(USER(), '@', -1) LIKE `Host`;");

    if(query->first())
        super_priv = query->value(0).toBool();
    else
        super_priv = 0;
}

SQueryLog::SQueryLog() :
    SQueryLog(new QSqlQuery(QSqlDatabase::database("connThird")))
{
}

SQueryLog::SQueryLog(QSqlDatabase db) :
    SQueryLog(new QSqlQuery(db))
{
}

SQueryLog::~SQueryLog()
{
    if(loggingActive)
        stop();
}

void SQueryLog::setFile(const QString &filename)
{
    file.setFileName(filename);
}

bool SQueryLog::truncateLog()
{
    bool r;

    query->exec("SHOW GLOBAL VARIABLES LIKE 'general_log'");
    if(query->first())
        generalLog = query->value(1).toString();
    if(generalLog != "ON")
        query->exec("SET GLOBAL general_log = 'ON';");

    query->exec("SHOW GLOBAL VARIABLES LIKE 'log_output'");
    if(query->first())
        logOutput = query->value(1).toString();
    if(logOutput != "TABLE")
        query->exec("SET GLOBAL log_output = 'TABLE';");

    r = query->exec("TRUNCATE mysql.general_log;");
    query->clear();
    return r;
}

bool SQueryLog::saveLog()
{
    // в файл выводим запросы к БД только для заданного соединения (перед запросом BEGIN нужно вызвать метод truncateLog(), чтобы файл содержал только интересующие запросы)
    QByteArray fileBuf = 0;

    if (file.open(QIODevice::ReadWrite))
    {
        if(super_priv)
        {
            query->exec(QString("SELECT "\
                                "  mysql.general_log.`argument` "\
                                "FROM"\
                                "  mysql.general_log "\
                                "WHERE "\
                                " `thread_id` IN (CONNECTION_ID())"
                                "  AND `user_host` LIKE '%%1%'"\
                                "  AND `argument` NOT IN ('')"\
                                "  AND `argument` NOT LIKE '%mysql.general_log%';")\
                        .arg(QSqlDatabase::database("connThird").userName()));

            while(query->next())
            {
                fileBuf.append(query->record().value(0).toByteArray());
                fileBuf.append(QString(";\n\n").toUtf8());
            }
        }
        else
            fileBuf.append("# Log unavailable because user has no SUPER privilege");

        file.resize(fileBuf.size());
        file.write(fileBuf);
        file.close();
    }
    else
        qDebug() << "Can't open log file";

    if(generalLog != "ON")
        query->exec(QString("SET GLOBAL general_log = '%1';").arg(generalLog));

    if(logOutput != "TABLE")
        query->exec(QString("SET GLOBAL log_output = '%1';").arg(logOutput));
    return 1;
}

void SQueryLog::start(const QString &className)
{
    Q_UNUSED(className);
#ifdef QT_DEBUG
    loggingActive = 1;
    setFile(SLocalSettings::appSettingsPath() + "/" + className + ".sql");
    if(super_priv)
        truncateLog();
#endif
}

/* Включение журналирования запросов для указанного соединения.
 * Возвращаемый указатель предназначен для выключения журналирования.
*/
SQueryLog *SQueryLog::start(QSqlDatabase db, const QString &className)
{
    SQueryLog *log = new SQueryLog(db);
    log->start(className);
    return log;
}

void SQueryLog::stop()
{
#ifdef QT_DEBUG
    saveLog();
    loggingActive = 0;
#endif
}
