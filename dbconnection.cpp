#include "dbconnection.h"

DBConnection* DBConnection::p_instance = 0;

DBConnection* DBConnection::getInstance( QObject* parent, const QString &username, const QString &password, const QString &ipaddr, const uint &port, const QString &dbName, const QString &connName )
{
if( !p_instance )
  p_instance = new DBConnection( parent, username, password, ipaddr, port, dbName, connName );
return p_instance;
}

void DBConnection::destroy()
{
if( p_instance )
  delete p_instance;

}

DBConnection::DBConnection( QObject* parent, const QString &username, const QString &password, const QString &ipaddr, const uint &port, const QString &dbName, const QString &connName ) : QObject( parent )
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    QSqlDatabase AppDB = QSqlDatabase::addDatabase("QMYSQL", connName);
    connectOK = 0;

    AppDB.setUserName(username);
    AppDB.setPassword(password);
    AppDB.setHostName(ipaddr);
    AppDB.setPort(port);
    AppDB.setDatabaseName(dbName);
//		AppDB.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5;MYSQL_OPT_READ_TIMEOUT=1");

    connectOK = AppDB.open();
    if (!connectOK)
    {
        qDebug("DB connect failed.");
//        emit this->DBConnectErr(AppDB.lastError().databaseText());
    }
    else
    {
        qDebug("DB successfully opened.");
//        emit this->DBConnectOK();
    }

}

DBConnection::~DBConnection()
{
    AppDB.close();
    AppDB.removeDatabase(AppDB.connectionName());
}
