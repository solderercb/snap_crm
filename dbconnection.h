#ifndef DBConnectionCONNECTION_H
#define DBConnectionCONNECTION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>

class DBConnection : public QObject
{
    Q_OBJECT

signals:
    void sgConnectErr(const QString &message);
    void sgConnectOK();

public:
    bool connectOK;
    static DBConnection* getInstance( QObject* parent=0, const QString &username = "", const QString &password = "", const QString &ipaddr = "127.0.0.1", const uint &port = 3306, const QString &dbName = "mysql", const QString &connName = "connMain" );
    static void destroy();


private:
    QSqlDatabase AppDB;
    static DBConnection* p_instance;
    DBConnection( QObject* parent, const QString &username, const QString &password, const QString &ipaddr, const uint &port, const QString &dbName, const QString &connName );
    ~DBConnection();
};

#endif // DBCONNECTION_H
