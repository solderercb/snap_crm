#include "windowsdispatcher.h"
#include "mainwindow.h" // подключать файл нужно именно здесь, по другому компилятор ругается
#include "chooseofficewindow.h"

windowsDispatcher::windowsDispatcher(QObject *parent) :
    QObject(parent)
{
    setObjectName("windowsDispatcherObj");
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
    companiesModel  = new QSqlQueryModel();
    officesModel    = new QSqlQueryModel();

    queryUserData->exec(QUERY_USER_DATA(QSqlDatabase::database("connMain").userName()));    // данные пользователя, которые потребуются ему в работе (кроме личных данных: телефон, фото, дата рождения, паспорт и т. п.)
    userData = new QMap<QString, QVariant>;
    queryUserData->first();

    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные пользователя могут быть дополнены (например, кодом текущего офиса, если у пользователя есть право выбора офиса при входе)
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД (например, ширины столбцов таблиц, которые могут иметь разные значения в случае если пользователь работает на разных ПК).
    // TODO: чтение найла настроек в AppData
    for (int i = 0; i < queryUserData->record().count(); i++)
    {
        userData->insert(queryUserData->record().fieldName(i), queryUserData->value(i));
    }

    queryPermissions->exec(QString("SELECT `permission_id` FROM `permissions_roles` WHERE `role_id` IN (%1)").arg(queryUserData->value("roles").toString()));
    permissions = new QMap<QString, bool>;
    while (queryPermissions->next())
    {
        permissions->insert(queryPermissions->value(0).toString(), 1);    // разрешённые пользователю действия хранятся в объекте QMap, не перечисленные действия не разрешены
    }

    // Список компаний.
    companiesModel->setQuery("SELECT  `name`,  `id`,  `type`,  `inn`,  `kpp`,  `ogrn`,  `ur_address`,  `site`,  `email`,  `logo`,  `banks`,  `is_default`,  `status`,  `director`,  `accountant`,  `tax_form`,  `description` FROM `companies` WHERE `id` = 1;", QSqlDatabase::database("connMain"));
    officesModel->setQuery(QString("SELECT  t1.`name`, CONCAT(t1.`name`, ' (', t2.`name`, ')') AS 'name_full',  t1.`id`,  t1.`state`,  `address`,  `phone`,  t1.`logo` AS 'off_logo',  t2.`logo` AS 'comp_logo',  `administrator`,  `created`,  `phone2`,  `default_company`,  `card_payment`,  `use_boxes`,  `paint_repairs`,  `warranty_sn` FROM `offices` AS t1 LEFT JOIN `companies` AS t2 ON t1.`default_company` = t2.`id` WHERE t1.`state` = 1 AND `default_company`  = %1;").arg(1), QSqlDatabase::database("connMain"));

    // TODO: добавить разрешение выбора компании при входе
    if (permissions->contains("59"))  // Менять офис при входе
    {
        createChooseOfficeWindow();
    }
    else
    {
        createMainWindow();
    }
}

void windowsDispatcher::createChooseOfficeWindow()
{
    chooseOfficeWindow *windowChooseOffice = new chooseOfficeWindow(this);
    QObject::connect(windowChooseOffice, SIGNAL(officeChoosed()), this, SLOT(createMainWindow()));
    windowChooseOffice->show();
}

void windowsDispatcher::createMainWindow()
{
    MainWindow *windowMain = MainWindow::getInstance(this); // указатель должен объявляться именно здесь, по другому компилятор ругается
    windowMain->show();
//    windowMain->createTabRepairs(); // по-умолчанию создаём вкладку Ремонты
    windowMain->createTabRepairNew(); // по-умолчанию создаём вкладку Ремонты
}

