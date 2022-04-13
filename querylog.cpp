#include "querylog.h"

queryLog::queryLog(QSqlQuery *q) :
    query(q)
{
}

void queryLog::setFile(const QString &filename)
{
    file.setFileName(filename);
}

bool queryLog::truncateLog()
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

bool queryLog::saveLog()
{
    // в файл выводим запросы к БД только для заданного соединения (перед запросом BEGIN нужно вызвать метод truncateLog(), чтобы файл содержал только интересующие запросы)
    QByteArray fileBuf = 0;

    if (file.open(QIODevice::ReadWrite))
    {
        query->exec(QString("SELECT "\
                    "  mysql.general_log.`argument` "\
                    "FROM"\
                    "  mysql.general_log "\
                    "WHERE "\
                    " `thread_id` IN (CONNECTION_ID())"
                    "  AND `user_host` LIKE '%%1%'"\
                    "  AND `argument` NOT LIKE '%mysql.general_log%';")\
                    .arg(QSqlDatabase::database().userName()));

        while(query->next())
        {

            fileBuf.append(query->record().value(0).toByteArray());
            fileBuf.append(QString(";\n\n").toUtf8());
        }
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
}
