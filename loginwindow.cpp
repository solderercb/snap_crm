#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "sqlcreds.h"

LoginWindow::LoginWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);

    QObject::connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(btnLoginHandler()));
    QObject::connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(btnCancelHandler()));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

#ifdef NO_LOGIN
void LoginWindow::debugLogin()
{
    btnLoginHandler();
}
#endif

void LoginWindow::btnLoginHandler()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);
    QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)

    connMain = QSqlDatabase::addDatabase("QMYSQL", "connMain");
    connections.append(&connMain);
    connNtfy = QSqlDatabase::addDatabase("QMYSQL", "connNtfy");
    connections.append(&connNtfy);
//    connThird = QSqlDatabase::addDatabase("QMYSQL", "connThird");     // в АСЦ сразу четыре соединения, зачем — ХЗ; понадобятся, тогда включим.
//    connections.append(&connThird);
//    connFourth = QSqlDatabase::addDatabase("QMYSQL", "connFourth");
//    connections.append(&connFourth);

    for (int i=0; i<connections.size(); i++)
    {
#ifdef NO_LOGIN
        connections[i]->setUserName(SQL_USER);
        connections[i]->setPassword(SQL_PSWD);
        connections[i]->setHostName(SQL_HOST);
        connections[i]->setPort(SQL_PORT);
        connections[i]->setDatabaseName(SQL_DB);
#else
        connections[i]->setUserName(ui->editLogin->text());
        connections[i]->setPassword(ui->editPassword->text());
        connections[i]->setHostName(ui->editIPaddr->text());
        connections[i]->setPort(ui->editPort->text().toUInt());
        connections[i]->setDatabaseName(ui->editDBName->text());
#endif
//		connections[i]->setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5;MYSQL_OPT_READ_TIMEOUT=1");
    }

    if (!connMain.open())
    {
        qDebug() << "DB connect failed: " << connMain.lastError().driverText();
        ui->labelStatus->setText(connMain.lastError().driverText());
        ui->labelStatus->setStyleSheet("color: red; font: bold;");
    }
    else
    {
        qDebug("DB successfully opened.");
        for (int i = 1; i<connections.size(); i++)  // открываем вспомогательные соединения
            connections[i]->open();
        emit this->DBConnectOK();
        this->hide();
        this->deleteLater();
    }
}

void LoginWindow::btnCancelHandler()
{
	emit this->btnCancelClick();
}
