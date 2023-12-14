#include "sapplication.h"
#include <QTranslator>
#include "windowsdispatcher.h"
//#include "mainwindow.h"

int main(int argc, char *argv[])
{
    SApplication app(argc, argv);
//    QApplication::setStyle("fusion");
    app.setFont(QFont("Segoe UI",9));  // Общий для всех виджетов шрифт (в win 7 тоже присутствует по умолчанию, но без эмоджи)
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    QTranslator translator;
    translator.load(QLocale(), "snap", "_", QCoreApplication::applicationDirPath()+"/lang/");
    app.installTranslator(&translator);

    windowsDispatcher *windowsDispatcherObj = new windowsDispatcher(nullptr);   // в main.cpp не может быть слотов, приходится создавать лишний объект

    QObject::connect(windowsDispatcherObj,SIGNAL(quit()),&app,SLOT(quit()));

    return app.exec();
}
