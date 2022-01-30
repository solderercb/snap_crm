#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include "windowsdispatcher.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    windowsDispatcher *windowsDispatcherObj = new windowsDispatcher(nullptr);   // в main.cpp не может быть слотов, приходится создавать лишний объект
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    LoginWindow *windowLogin = new LoginWindow();

    QObject::connect(windowLogin,SIGNAL(DBConnectOK()),windowsDispatcherObj,SLOT(connectOK()));
    QObject::connect(windowLogin,SIGNAL(btnCancelClick()),&app,SLOT(quit()));

    windowLogin->show();
#ifdef NO_LOGIN     // NO_LOGIN объявляется в loginwindow.h
    windowLogin->debugLogin();
#endif


	return app.exec();
}
