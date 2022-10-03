#include "global.h"
#include "appver.h"
#include "windowsdispatcher.h"
#include "loginwindow.h"
#include "mainwindow.h" // подключать файл нужно именно здесь, по другому компилятор ругается
#include "chooseofficewindow.h"
#define AUTO_CHOOSE_OFFICE
#ifdef QT_DEBUG
#ifdef AUTO_CHOOSE_OFFICE
#define AUTO_CHOOSE_OFFICE_
#endif
#endif

windowsDispatcher::windowsDispatcher(QObject *parent) :
    QObject(parent)
{
    setObjectName("windowsDispatcherObj");

    appLog->appendRecord(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " Application start");
    LoginWindow *windowLogin = new LoginWindow(this);

    QObject::connect(windowLogin,SIGNAL(DBConnectOK()),this,SLOT(connectOK()));
    QObject::connect(windowLogin,SIGNAL(btnCancelClick()),this,SIGNAL(quit()));

    windowLogin->show();
#ifdef NO_LOGIN_     // NO_LOGIN объявляется в loginwindow.h
    windowLogin->debugLogin();
#endif

}

windowsDispatcher::~windowsDispatcher()
{
}

void windowsDispatcher::connectOK()
{
    initSystemObjects();
    initUserDbData();
    initPermissions();
    initCompanies();
    initOffices();

    userActivityLog->updateLoginTimestamp();
    userActivityLog->appendRecord(tr("Login"));   // Заменено на "Login", потому что АСЦ не позволяет запускать два экз. программы, а определение происходит по фразе "Выполнен вход в систему"

    // TODO: добавить разрешение выбора компании при входе
    if (permissions->contains("59"))  // Менять офис при входе
    {
        createChooseOfficeWindow();
    }
    else
    {
        userDbData->insert("current_office", officesModel->record(userDbData->value("office").toInt()-1).value("id").toInt());
        userDbData->insert("current_office_name", officesModel->record(userDbData->value("office").toInt()-1).value("name").toString());
        createMainWindow();
    }
    userDbData->insert("company", 1);   // TODO: несколько компаний
}

void windowsDispatcher::createChooseOfficeWindow()
{
#ifdef AUTO_CHOOSE_OFFICE_
#define AUTO_OFFICE_ID 0
    userDbData->insert("current_office", officesModel->record(AUTO_OFFICE_ID).value("id").toInt());
    userDbData->insert("current_office_name", officesModel->record(AUTO_OFFICE_ID).value("name").toString());
    createMainWindow();
#else
    chooseOfficeWindow *windowChooseOffice = new chooseOfficeWindow(this);
    QObject::connect(windowChooseOffice, SIGNAL(officeChoosed()), this, SLOT(createMainWindow()));
    windowChooseOffice->show();
#endif
}

void windowsDispatcher::createMainWindow()
{
    MainWindow *windowMain = MainWindow::getInstance(this); // указатель должен объявляться именно здесь, по другому компилятор ругается
    windowMain->show();
    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
//    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты
}

