#include "tdebugstuff.h"
#include "qtestcase.h"
#include <iostream>
#include <QTextCodec>
#include <TdConn>
#include <ProjectGlobals>

QString TDebugStuff::m_settingsPath;

TDebugStuff::TDebugStuff()
{
    readSettings();
    connect();
}

void TDebugStuff::readSettings()
{
    QFile file;
    file.setFileName(m_settingsPath);
    QVERIFY2(file.exists(), QString("%1 not found").arg(QFileInfo(file).absoluteFilePath()).toLocal8Bit());

    if(!file.exists())
        QSKIP("");
    debugOptions = std::make_shared<QSettings>(file.fileName(), QSettings::IniFormat);
    debugOptions->setIniCodec(QTextCodec::codecForName("UTF-8"));
}

void TDebugStuff::connect()
{
    QVariant tmp;
    QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
    if(debugOptions.get() == nullptr)
        return;

    if (debugOptions->childGroups().contains("debugLogin"))
    {
        debugOptions->beginGroup("debugLogin");

        QStringList allKeys = debugOptions->allKeys();
        for(int i = 0; i < allKeys.size(); i++)
        {
            tmp = debugOptions->value(allKeys.at(i));
            if(tmp.isValid())
            {
                loginCreds->insert(allKeys.at(i), tmp);
            }

        }
        debugOptions->endGroup();

        connections.clear();
        connMain = QSqlDatabase::addDatabase("QMYSQL", TdConn::main());
        connections.append(&connMain);
        connThird = QSqlDatabase::addDatabase("QMYSQL", TdConn::session());
        connections.append(&connThird);

        for (int i=0; i<connections.size(); i++)
        {
            connections[i]->setUserName(loginCreds->value("user", "user").toString());
            connections[i]->setPassword(loginCreds->value("password", "password").toString());
            connections[i]->setHostName(loginCreds->value("host", "127.0.0.1").toString());
            connections[i]->setPort(loginCreds->value("port", 3306).toInt());
            connections[i]->setDatabaseName(loginCreds->value("database", "service").toString());
        }

        for (int i = 0; i<connections.size(); i++)  // открываем соединения
            connections[i]->open();
        if (connMain.open())
        {
            // установка режимов; начиная с MySQL v5.7.5 по умолчанию включен режим  ONLY_FULL_GROUP_BY
            QSqlQuery querySetSqlMode = QSqlQuery(QSqlDatabase::database(TdConn::main()));
            querySetSqlMode.exec("SET SESSION sql_mode = sys.list_drop(@@session.sql_mode, 'ONLY_FULL_GROUP_BY');");

            auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));

            query->exec("SELECT `version`, `version_snap` FROM `config` WHERE `id` = 1;");
            query->first();
            Q_ASSERT_X(query->value(0).toString() == "3.7.37.1184", "TDebugStuff", "Попытка запуска теста(-ов) с подключением к базе неподходящей версии");
//            Q_ASSERT_X(query->value(0).toString() == "0.0.0.304", "TDebugStuff", "Попытка запуска теста(-ов) с подключением к базе неподходящей версии");

            query->exec("SELECT * FROM `schemaversions` WHERE `scriptname` = 'SNAP.TEST';");
            Q_ASSERT_X(query->first(), "TDebugStuff", "Попытка запуска теста(-ов) с подключением к базе с неподходящим набором данных");
        }
    }
}

void TDebugStuff::setSettingsPath(const QString &settingsPath)
{
    m_settingsPath = settingsPath;
}

/* Инициализация контейнера QMap данными секции table для выполнения тестирования.
 * Для инициализации testData необходимо наличие в БД таблицы table и записи в этой
 * таблице с указанным id
 *
 * Формат:
 * [<название таблицы>]
 * id=<число>
 * <параметр>=<значение>
 * ...
 *
 * Возвращает 1 если указанная запись есть в БД
*/
bool TDebugStuff::testRecordExists(const QString &table, QMap<QString, QVariant> &testData)
{
    if(!debugOptions->childGroups().contains(table))
        return 0;

    debugOptions->beginGroup(table);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto id = debugOptions->value("id");

    if(id.isValid())
    {
        QString q = QString("SELECT * FROM `%1` WHERE `id` IN (%2);").arg(table).arg(id.toInt());

        query->exec(q);

        if(query->first())
        {
            foreach (auto key, debugOptions->childKeys())
            {
                testData.insert(key, debugOptions->value(key));
            }
            debugOptions->endGroup();
            return 1;
        }
    }

    debugOptions->endGroup();
    return 0;
}

/* Инициализация контейнера QMap данными секции section для выполнения тестирования.
 * В отличие от TDebugStuff::testRecordExists(), для инициализации testData достаточно
 * наличия указанной секции section
 *
 * Формат:
 * [<название секции>]
 * <параметр>=<значение>
 * ...
 *
 * Возвращает 0 если указанная секция в файле отсутсвует
*/
bool TDebugStuff::readTestData(const QString &section, QMap<QString, QVariant> &testData)
{
    if(!debugOptions->childGroups().contains(section))
        return 0;

    debugOptions->beginGroup(section);
    foreach (auto key, debugOptions->childKeys())
    {
        testData.insert(key, debugOptions->value(key));
    }
    debugOptions->endGroup();
    return 1;
}

void TDebugStuff::startSqlLog(const QSqlDatabase &database, const QString &className)
{
    auto q = new QSqlQuery(database);
    queryLog = std::make_unique<SQueryLog>(q);
    this->query.reset(q);    // это позволяет удержать QSqlQuery в куче и корректно выполниться SQueryLog::saveLog()
    queryLog->start(className);
}
