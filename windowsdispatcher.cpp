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
    delete queryPermissions;
}

void windowsDispatcher::connectOK()
{
    userDataModel->setQuery(QUERY_SEL_USER_DATA(QSqlDatabase::database("connMain").userName()), QSqlDatabase::database("connMain")); // использую QSqlQueryModel потому, что она ещу будет использоваться (например, в отчетах)

    queryPermissions= new QSqlQuery(QSqlDatabase::database("connMain"));

    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные пользователя могут быть дополнены (например, кодом текущего офиса, если у пользователя есть право выбора офиса при входе)
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД (например, ширины столбцов таблиц, которые могут иметь разные значения в случае если пользователь работает на разных ПК).
    for (int i = 0; i < userDataModel->record(0).count(); i++)
    {
        userData->insert(userDataModel->record(0).fieldName(i), userDataModel->record(0).value(i));
    }

    queryPermissions->exec(QUERY_SEL_PERMISSIONS(userData->value("roles").toString()));
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
#ifdef AUTO_CHOOSE_OFFICE_
#define AUTO_OFFICE_ID 0
    userData->insert("current_office", officesModel->record(AUTO_OFFICE_ID).value("id").toInt());
    userData->insert("current_office_name", officesModel->record(AUTO_OFFICE_ID).value("name").toString());
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
//    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
//    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты
}

