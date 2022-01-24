#include "loginwindow.h"
#include "mainwindow.h"
#include "my_widget.h"
#include <QApplication>
#define NO_LOGIN

void InitDBConnectionClass();

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    LoginWindow *windowLogin = new LoginWindow;
    MainWindow *windowMain = MainWindow::getInstance();
//    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты

    QObject::connect(windowLogin,SIGNAL(ConnectToDB(QString,QString,QString, uint,QString,QString)),windowMain,SLOT(ConnectToDB(QString,QString,QString, uint,QString,QString)));
    QObject::connect(windowMain,SIGNAL(DBConnectErr(QString)),windowLogin,SLOT(DBConnectErr(QString)));
    QObject::connect(windowMain,SIGNAL(DBConnectOK()),windowMain,SLOT(show()));
    QObject::connect(windowMain,SIGNAL(DBConnectOK()),windowLogin,SLOT(close()));
    QObject::connect(windowLogin,SIGNAL(btnCancelClick()),&app,SLOT(quit()));



#ifdef NO_LOGIN
//    windowMain->showMaximized();
    windowMain->show();
#elif
    windowLogin->show();
#endif

	return app.exec();
}
