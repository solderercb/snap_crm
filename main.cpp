#include <QApplication>
#include "windowsdispatcher.h"
//#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    QApplication::setStyle("fusion");
    app.setFont(QFont("Segoe UI",9));  // Общий для всех виджетов шрифт (в win 7 тоже присутствует по умолчанию)
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);
    windowsDispatcher *windowsDispatcherObj = new windowsDispatcher(nullptr);   // в main.cpp не может быть слотов, приходится создавать лишний объект

    QObject::connect(windowsDispatcherObj,SIGNAL(quit()),&app,SLOT(quit()));

	return app.exec();
}
