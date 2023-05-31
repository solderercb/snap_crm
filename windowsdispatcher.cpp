#include "global.h"
#include "appver.h"
#include "windowsdispatcher.h"
#include "loginwindow.h"
#include "mainwindow.h" // подключать файл нужно именно здесь, по другому компилятор ругается
#include "chooseofficewindow.h"

windowsDispatcher::windowsDispatcher(QObject *parent) :
    QObject(parent)
{
    setObjectName("windowsDispatcherObj");

    debugInitSettings();
    appLog->appendRecord(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " Application start");
    LoginWindow *windowLogin = new LoginWindow(this);

    QObject::connect(windowLogin,SIGNAL(DBConnectOK()),this,SLOT(connectOK()));
    QObject::connect(windowLogin,SIGNAL(btnCancelClick()),this,SIGNAL(quit()));

    windowLogin->show();

    if(debugLoginOptions)
        windowLogin->debugLogin();
}

windowsDispatcher::~windowsDispatcher()
{
}

void windowsDispatcher::debugInitSettings()
{
    QFile file;
    QDir appDataDir = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));   // C:/Users/user/AppData/Local
    file.setFileName(SLocalSettings::appSettingsPath()+"/../debug.ini");

    if (file.exists())
    {
        debugOptions = new  QSettings(file.fileName(), QSettings::IniFormat);
        debugOptions->setProperty("fileName", file.fileName());
    }
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

    if(debugLoginOptions)
        delete debugLoginOptions;
}

void windowsDispatcher::createChooseOfficeWindow()
{
    if(debugLoginOptions)
        if(debugLoginOptions->contains("office"))
        {
            userDbData->insert("current_office", officesModel->record(debugLoginOptions->value("office").toInt()).value("id").toInt());
            userDbData->insert("current_office_name", officesModel->record(debugLoginOptions->value("office").toInt()).value("name").toString());
            createMainWindow();
            return;
        }

    chooseOfficeWindow *windowChooseOffice = new chooseOfficeWindow(this);
    QObject::connect(windowChooseOffice, SIGNAL(officeChoosed()), this, SLOT(createMainWindow()));
    windowChooseOffice->show();
}

void windowsDispatcher::createMainWindow()
{
    MainWindow *windowMain = MainWindow::getInstance(this); // указатель должен объявляться именно здесь, по другому компилятор ругается
    windowMain->show();
//    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
//    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты
}

