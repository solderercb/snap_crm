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
    initUserDbData();
    initPermissions();
    initCompanies();
    initOffices();

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

    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;
    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_LOGIN(userDbData->value("id").toString()));
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userDbData->value("id").toString()));
//            QUERY_EXEC(query, nDBErr)(QUERY_INS_USER_ACTIVITY(QString("Выполнен вход в систему")));   // Пока отключено, потому что АСЦ не позволяет два логина
    QUERY_COMMIT_ROLLBACK(query, nDBErr);
    delete query;
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

