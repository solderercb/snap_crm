#include "appver.h"
#include "windowsdispatcher.h"
#include "loginwindow.h"
#include "mainwindow.h" // подключать файл нужно именно здесь, по другому компилятор ругается
#include "chooseofficewindow.h"
#ifdef QT_DEBUG
    #define AUTO_CHOOSE_OFFICE
#endif

windowsDispatcher::windowsDispatcher(QObject *parent) :
    QObject(parent)
{
    setObjectName("windowsDispatcherObj");
    userData = new QMap<QString, QVariant>;
    userLocalData = new QMap<QString, QVariant>;
    permissions = new QMap<QString, bool>;
    companiesModel  = new QSqlQueryModel();
    officesModel    = new QSqlQueryModel();

    LoginWindow *windowLogin = new LoginWindow(this);

    QObject::connect(windowLogin,SIGNAL(DBConnectOK()),this,SLOT(connectOK()));
    QObject::connect(windowLogin,SIGNAL(btnCancelClick()),this,SIGNAL(quit()));

    windowLogin->show();
#ifdef QT_DEBUG
#ifdef NO_LOGIN     // NO_LOGIN объявляется в loginwindow.h
    windowLogin->debugLogin();
#endif
#endif

}

windowsDispatcher::~windowsDispatcher()
{
    delete queryPermissions;
    delete companiesModel;
    delete officesModel;
}

void windowsDispatcher::connectOK()
{
    queryUserData   = new QSqlQuery(QSqlDatabase::database("connMain"));
    queryPermissions= new QSqlQuery(QSqlDatabase::database("connMain"));

    queryUserData->exec(QUERY_SEL_USER_DATA(QSqlDatabase::database("connMain").userName()));    // данные пользователя, которые потребуются ему в работе (кроме личных данных: телефон, фото, дата рождения, паспорт и т. п.)
    queryUserData->first();

    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные пользователя могут быть дополнены (например, кодом текущего офиса, если у пользователя есть право выбора офиса при входе)
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД (например, ширины столбцов таблиц, которые могут иметь разные значения в случае если пользователь работает на разных ПК).
    // TODO: чтение найла настроек в AppData
    for (int i = 0; i < queryUserData->record().count(); i++)
    {
        userData->insert(queryUserData->record().fieldName(i), queryUserData->value(i));
    }

    queryPermissions->exec(QUERY_SEL_PERMISSIONS(queryUserData->value("roles").toString()));
    while (queryPermissions->next())
    {
        permissions->insert(queryPermissions->value(0).toString(), 1);    // разрешённые пользователю действия хранятся в объекте QMap, не перечисленные действия не разрешены
    }

    // Список компаний.
    companiesModel->setQuery(QUERY_SEL_COMPANIES, QSqlDatabase::database("connMain"));
    officesModel->setQuery(QUERY_SEL_OFFICES(1), QSqlDatabase::database("connMain"));

    // TODO: добавить разрешение выбора компании при входе
    if (permissions->contains("59"))  // Менять офис при входе
    {

        createChooseOfficeWindow();
    }
    else
    {
        userData->insert("current_office", officesModel->record(userData->value("office").toInt()-1).value("id").toInt());
        userData->insert("current_office_name", officesModel->record(userData->value("office").toInt()-1).value("name").toString());
        createMainWindow();
    }
}

void windowsDispatcher::createChooseOfficeWindow()
{
#ifndef QT_DEBUG
    chooseOfficeWindow *windowChooseOffice = new chooseOfficeWindow(this);
    QObject::connect(windowChooseOffice, SIGNAL(officeChoosed()), this, SLOT(createMainWindow()));
    windowChooseOffice->show();
#else
#ifdef AUTO_CHOOSE_OFFICE
#define AUTO_OFFICE_ID 0
    userData->insert("current_office", officesModel->record(AUTO_OFFICE_ID).value("id").toInt());
    userData->insert("current_office_name", officesModel->record(AUTO_OFFICE_ID).value("name").toString());
    createMainWindow();
#endif
#endif
}

void windowsDispatcher::createMainWindow()
{
    MainWindow *windowMain = MainWindow::getInstance(this); // указатель должен объявляться именно здесь, по другому компилятор ругается
    windowMain->show();
//    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
//    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты
}

