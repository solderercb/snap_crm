#include "windowsdispatcher.h"
#include "mainwindow.h"

windowsDispatcher::windowsDispatcher(QObject *parent) :
    QObject(parent)
{
    int i;
}

void windowsDispatcher::connectOK()
{
    qDebug() << "windowsDispatcher::connectOK()";
    MainWindow *windowMain = MainWindow::getInstance();
    windowMain->show();
    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
//    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты
}

