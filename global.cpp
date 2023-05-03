#include "global.h"
#include "com_sql_queries.h"
#include "models/sclientmodel.h"

QLocale sysLocale = QLocale::system();
QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
QMap<QString, QVariant> *userDbData = new QMap<QString, QVariant>;
QSqlQueryModel *userDbDataModel = new QSqlQueryModel();
SLocalSettings *localSettings = new SLocalSettings();
t_userSettings *userLocalData = nullptr;
QMap<QString, bool> *permissions = new QMap<QString, bool>;
QMap<QString, QVariant> *comSettings = new QMap<QString, QVariant>;
SSqlQueryModel *clientPhoneTypesModel = new SSqlQueryModel();
SSqlQueryModel *companiesModel = new SSqlQueryModel;
SSqlQueryModel *officesModel = new SSqlQueryModel;
QSqlQueryModel *warehousesModel = new QSqlQueryModel;
SSqlQueryModel *allUsersModel = new SSqlQueryModel;
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
SSqlQueryModel *usersModel = new SSqlQueryModel;
SSqlQueryModel *usersSalaryTaxesModel = new SSqlQueryModel;
SSqlQueryModel *managersModel = new SSqlQueryModel;
SSqlQueryModel *engineersModel = new SSqlQueryModel;
SSqlQueryModel *itemBoxesModel = new SSqlQueryModel;
SSqlQueryModel *repairBoxesModel = new SSqlQueryModel;
SSqlQueryModel *paymentSystemsModel = new SSqlQueryModel;
SPaymentTypesModel *receiptTypesModel;
SPaymentTypesModel *expenditureTypesModel;
QStandardItemModel* clientsTypesList = new QStandardItemModel;
SSqlQueryModel* clientAdTypesList = new SSqlQueryModel;
SStandardItemModel *statusesModel = new SStandardItemModel();
qint64 repairTermSeconds;
SStandardItemModel *notifyStatusesModel = new SStandardItemModel();
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();
QMap<int, QString> *warrantyTermsMap = new QMap<int, QString>;

// фактическая высота ComboBox'а будет на 4 пикселя больше; высоту нужно задать в styleSheet, т. к. Qt5 и Qt6 без этого параметра рисуют виджеты чуть-чуть разной высоты
QString commonComboBoxStyleSheet = "QComboBox {  height: 18px; border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonComboBoxStyleSheetRed = "QComboBox {  height: 18px; border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";

QString commonLineEditStyleSheet = "QLineEdit {  height: 18px; border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonLineEditStyleSheetRed = "QLineEdit {  height: 18px; border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonTextEditStyleSheet = "QTextEdit {  border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonTextEditStyleSheetRed = "QTextEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonDateEditStyleSheet = "QDateEdit {  border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonDateEditStyleSheetRed = "QDateEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonSpinBoxStyleSheet = "QSpinBox, QDoubleSpinBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonSpinBoxStyleSheetRed = "QSpinBox, QDoubleSpinBox {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
SStandardItemModel *rejectReasonModel = new SStandardItemModel;
SStandardItemModel *priceColModel = new SStandardItemModel;
SStandardItemModel *itemUnitsModel = new SStandardItemModel;
SAppLog *appLog = new SAppLog();
SUserActivityModel *userActivityLog;
SStandardItemModel *clientBinaryProperties;
QSettings *debugOptions = nullptr;
QMap<QString, QVariant> *debugLoginOptions = nullptr;
SStandardItemModel* storeItemsIntReserveStatesModel = new SStandardItemModel;

//QWidget *modalWidget = nullptr;

bool readStatuses(QStandardItemModel &model, QJsonArray &jsonArray)
{
    QList<QStandardItem*> *list;
    repairTermSeconds = 0;

    if (!jsonArray.empty())
    {
        QJsonArray::iterator i = jsonArray.begin();
        do
        {
            QJsonObject jsonObj = i->toObject();
            list = new QList<QStandardItem*>;
            int statusTermDays = 0;
            int statusTermSecons = 0;
            QTime statusTermTime;

            QString statusTermStr = jsonObj["Terms"].toString();  // термин полностью

            *list << new QStandardItem(jsonObj["Name"].toString())\
                  << new QStandardItem(QString::number(jsonObj["Id"].toInt()))\
                  << new QStandardItem(jsonObj["Color"].toString())\
                  << new QStandardItem(statusTermStr)\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Contains"],"|"))\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Actions"],"|"))\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Roles"],"|"));

            QString statusTermDaysStr = statusTermStr;  // дни отдельно
            statusTermDaysStr.chop(9);   // удаление части ".hh:mm:ss"; если дни не заданы, то результатом будет пустая строка
            statusTermDays = statusTermDaysStr.toInt();
            repairTermSeconds += statusTermDays;
            statusTermStr = statusTermStr.right(8);    // оставить только часы:минуты:секунды
            statusTermTime = QTime::fromString(statusTermStr, "hh:mm:ss");
            statusTermSecons = QTime(0, 0, 0).secsTo(statusTermTime);
            *list << new QStandardItem(QString::number(statusTermDays*24*3600 + statusTermSecons));

            model.appendRow(*list);
            i++;
        }
        while ( i != jsonArray.end());
    }
    if(repairTermSeconds < 2)
        repairTermSeconds = 0;  // на случай, если в таблице нет статусов с длительностью в несколько дней
    else
        repairTermSeconds = (repairTermSeconds*24 - 30)*3600;
    return 1;
}

/* Объединение элементов массива в строку с разделителем "|".
 * Такой формат позволит использовать полученный список в качестве
 * шаблона регулярного выражения (в частности, при переключении статуса ремонта).
 */
QString jsonArrayJoin(QJsonValue value, const QString)
{
    if(value.isArray())
    {
        QString out = "";
        QVariantList array = value.toArray().toVariantList();
        if (!array.empty())
        {
            QVariantList::iterator i = array.begin();
            do
            {
                if (out != "")
                    out += "|";
                out += i->toString();
                i++;
            }
            while ( i != array.end());
        }
        return out;
    }
    return "not an array";
}

void initGlobalModels()
{
    QString query;

    QSqlQuery *queryCommonSettings = new QSqlQuery(QSqlDatabase::database("connMain"));

    queryCommonSettings->exec(QUERY_SEL_COMMON_SETTINGS);
    comSettings = new QMap<QString, QVariant>;
    queryCommonSettings->first();
    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные общих настроек могут быть дополнены.
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД.
    // TODO: чтение файла настроек в AppData
    // TODO: чтение файлов настроек пользователя из каталога приложения (например, program files): RepairsGrid-user.xml, SaleGrid-user.xml и многие другие
    for (int i = 0; i < queryCommonSettings->record().count(); i++)
    {
        comSettings->insert(queryCommonSettings->record().fieldName(i), queryCommonSettings->value(i));
    }
    queryCommonSettings->exec(QUERY_SEL_COMMON_SETTINGS2);  // вторая "порция" общих настроек ( в ASC CRM настроки из табил. config начиная с v3.7.18.969 schemaversion 258 постепенно "переезжают" в табл. settings; нова ятаблица имеет приоритет
    while(queryCommonSettings->next())
    {
        comSettings->insert(queryCommonSettings->value(0).toString(), queryCommonSettings->value(1));
    }

    for (int i=0; i < comSettings->value("phone_mask1").toString().size(); i++)     // В ASC CRM (а точнее в DevExpress) маски ввода в lineEdit работают не так, как в Qt
        if (comSettings->value("phone_mask1").toString().at(i) == '0')              // В Qt: 0 - это необязательная цифра, а 9 — обязательная цифра. Чтобы работала проверка
        {                                                                           // введённого номера телефона (если в настройках установлен флаг обязательности), нужно изменить маску.
            qDebug() << "МАСКА ТЕЛЕФОНА 1!!!";                                      // (это заглушка для отладки)
            break;                                                                  // TODO: добавить хитрую подсказку для полей ввода номера, если обнаруживается такое несовпадение
        }
    for (int i=0; i < comSettings->value("phone_mask2").toString().size(); i++)
        if (comSettings->value("phone_mask2").toString().at(i) == '0')
        {
            qDebug() << "МАСКА ТЕЛЕФОНА 2!!!";
            break;
        }
    comSettings->insert("phone_mask1", comSettings->value("phone_mask1").toString().replace('0', '9')); // предупреждалка пусть орёт, но чтобы работала проверка, изменим маску внаглую
    comSettings->insert("phone_mask2", comSettings->value("phone_mask2").toString().replace('0', '9'));


//    QList<QStandardItem*> *clientPhoneTypesSelector;
//    clientPhoneTypesSelector = new QList<QStandardItem*>();
//    *clientPhoneTypesSelector << new QStandardItem("мобильный") << new QStandardItem("1") << new QStandardItem(comSettings->value("phone_mask1").toString()); // в ASC формат задаётся нулями, но в поиске совпадающих клиентов  это предусмотрено
//    clientPhoneTypesModel->appendRow( *clientPhoneTypesSelector );
//    clientPhoneTypesSelector = new QList<QStandardItem*>();;
//    *clientPhoneTypesSelector << new QStandardItem("городской") << new QStandardItem("2") << new QStandardItem(comSettings->value("phone_mask2").toString());
//    clientPhoneTypesModel->appendRow( *clientPhoneTypesSelector );
    clientPhoneTypesModel->setQuery(QUERY_SEL_PHONE_TYPES, QSqlDatabase::database("connMain"));
    clientPhoneTypesModel->setObjectName("clientPhoneTypesModel");
//    clientPhoneTypesModel->setHorizontalHeaderLabels({"name", "id", "mask"});

    warehousesModel->setQuery(QUERY_SEL_WAREHOUSES(userDbData->value("current_office").toInt()), QSqlDatabase::database("connMain"));
    warehousesModel->setObjectName("warehousesModel");
    allUsersModel->setQuery(QUERY_SEL_ALL_USERS, QSqlDatabase::database("connMain"));
    allUsersModel->setDisplayRoleColumn("username");
    allUsersModel->setObjectName("allUsersModel");
    for(int i = 0; i < allUsersModel->rowCount(); i++)
    {
        allUsersMap->insert(allUsersModel->record(i).value("id").toInt(), allUsersModel->record(i).value("username").toString());
    }
    usersModel->setQuery(QUERY_SEL_USERS, QSqlDatabase::database("connMain"));
    usersModel->setDisplayRoleColumn("username");
    usersModel->setObjectName("usersModel");
    usersSalaryTaxesModel->setQuery(QUERY_SEL_USERS_SALARY_TAXES, QSqlDatabase::database("connMain"));
    usersSalaryTaxesModel->setObjectName("usersSalaryTaxesModel");
    managersModel->setQuery(QUERY_SEL_MANAGERS, QSqlDatabase::database("connMain"));
    managersModel->setDisplayRoleColumn("username");
    managersModel->setObjectName("managersModel");
    engineersModel->setQuery(QUERY_SEL_ENGINEERS, QSqlDatabase::database("connMain"));
    engineersModel->setDisplayRoleColumn("username");
    engineersModel->setObjectName("engineersModel");
    itemBoxesModel->setQuery(QUERY_SEL_ITEM_BOXES(userDbData->value("current_office").toInt()), QSqlDatabase::database("connMain"));
    itemBoxesModel->setObjectName("itemBoxesModel");
    repairBoxesModel->setQuery(QUERY_SEL_REPAIR_BOXES, QSqlDatabase::database("connMain"));
    repairBoxesModel->setObjectName("repairBoxesModel");

    // TODO: нужна прокси-модель для отображения платёжных систем в соответствии с правами пользователя
    // а также таблица payment_systems_users, содержащая права на видимость
    paymentSystemsModel->setQuery(QUERY_SEL_PAYMENT_SYSTEMS, QSqlDatabase::database("connMain"));
    paymentSystemsModel->setObjectName("paymentSystemsModel");

    receiptTypesModel = new SPaymentTypesModel(0);
    receiptTypesModel->setObjectName("receiptTypesModel"); // TODO: возможно, тоже нужны прокси-модели для отображения типов ПКО и РКО в соответствии с правами пользователя
    expenditureTypesModel = new SPaymentTypesModel(1);
    expenditureTypesModel->setObjectName("expenditureTypesModel");

    clientAdTypesList->setQuery(QUERY_SEL_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));
    clientAdTypesList->setObjectName("clientAdTypesList");

    QVector<QString> clientTypesList = {"Все клиенты", "Организации", "Посредники", "Поставщики", "Постоянные клиенты", "Проблемные клиенты", "Реализаторы"};
    QVector<QString> clientTypesQueryFilterList = {"`type` IN (0, 1)", "`type` = 1", "`is_agent` = 1", "`is_dealer` = 1", "`is_regular` = 1", "`is_bad` = 1", "`is_realizator` = 1"};
    for (int i=0; i<clientTypesList.size(); i++)
    {
        QList<QStandardItem*> *clientTypeSelector = new QList<QStandardItem*>();
        *clientTypeSelector << new QStandardItem(clientTypesList.at(i)) << new QStandardItem(QString::number(i)) << new QStandardItem(clientTypesQueryFilterList.at(i));
        clientsTypesList->appendRow(*clientTypeSelector);
    }

    QJsonDocument jsonDoc(QJsonDocument::fromJson(comSettings->value("statuses").toByteArray()));
    if (jsonDoc.isArray())
    {
        QJsonArray jsonArray(jsonDoc.array());
        readStatuses(*statusesModel, jsonArray);
    }
    else
    {
        QList<QStandardItem*> *el = new QList<QStandardItem*>({new QStandardItem("Model init failed")});
        statusesModel->appendRow(*el);
    }
    statusesModel->setHorizontalHeaderLabels({"name","id","color","terms","contains","actions","roles"});

    QVector<QString> notifyStatusesList = {"---", "Клиент оповещён", "Клиент не отвечает", "Клиент не доступен", "Не оповещён прочее"};
    for (int i=0; i<notifyStatusesList.size(); i++)
    {
        QList<QStandardItem*> *notifyStatusSelector = new QList<QStandardItem*>();
        *notifyStatusSelector << new QStandardItem(notifyStatusesList.at(i)) << new QStandardItem(QString::number(i));
        notifyStatusesModel->appendRow(*notifyStatusSelector);
    }
    notifyStatusesModel->setHorizontalHeaderLabels({"name","id"});

    // TODO: В АСЦ в списке сроков гарантии также присутствуют "Согласно гарантии производителя", "Согласно ЗоЗПП" и "Согласно ФГТ", но, похоже, их выбор не реализован
    QVector<QString> warrantyTermsList = {"нет", "7 дней", "14 дней", "1 мес", "2 мес", "3 мес", "4 мес", "6 мес", "1 год", "2 года", "3 года"};
    int warrantyTerms[] = {0, 7, 14, 31, 62, 93, 124, 186, 365, 730, 1095};
    // Модель для комбобоксов при редактировании, а QMap для отображения
    for (int i=0; i<warrantyTermsList.size(); i++)
    {
        QList<QStandardItem*> *warrantyTermSelector = new QList<QStandardItem*>();
        *warrantyTermSelector << new QStandardItem(warrantyTermsList.at(i)) << new QStandardItem(QString::number(warrantyTerms[i]));
        warrantyTermsModel->appendRow(*warrantyTermSelector);
        warrantyTermsMap->insert(warrantyTerms[i], warrantyTermsList.at(i));
    }
    warrantyTermsModel->setObjectName("warrantyTermsModel");
    warrantyTermsModel->setHorizontalHeaderLabels({"name","days"});


    QVector<QString> rejectReasonList = {"отказ от ремонта", "ремонт не возможен", "ремонт не возможен из-за отсутствия запчастей", "ремонт не рентабелен", "неисправносте не проявилась", "другие причины"};
    rejectReasonModel->setProperty("other_reject_reason", 5); // более элегантный способ не придумал (такой, чтобы задавать id причины отказа в "одном месте" и чтобы это не поломалось при переводе)
    QList<QStandardItem*> *rejectReasonSelector;
    for (int i=0; i<rejectReasonList.size(); i++)
    {
        rejectReasonSelector = new QList<QStandardItem*>();
        *rejectReasonSelector << new QStandardItem(rejectReasonList.at(i)) << new QStandardItem(QString::number(i));
        rejectReasonModel->appendRow(*rejectReasonSelector);
    }
    rejectReasonModel->setObjectName("rejectReasonModel");
    rejectReasonModel->setHorizontalHeaderLabels({"name","id"});

    QVector<QString> priceColNamesList = {"Цена для сервиса", "Цена розница", "Цена опт", "Цена опт2", "Цена опт3"};
    QVector<QString> priceColIdsList = {"1", "2", "3", "4", "5"};
    QVector<QString> priceColDBFieldsList = {"price", "price2", "price3", "price4", "price5"};
    QList<QStandardItem*> *priceColSelector;
    for (int i=0; i<priceColNamesList.size(); i++)
    {
        priceColSelector = new QList<QStandardItem*>();
        *priceColSelector << new QStandardItem(priceColNamesList.at(i)) << new QStandardItem(priceColIdsList.at(i)) << new QStandardItem(priceColDBFieldsList.at(i));
        priceColModel->appendRow(*priceColSelector);
    }
    priceColModel->setObjectName("priceColModel");
    priceColModel->setHorizontalHeaderLabels({"name", "id", "dbColumn"});

    QVector<QString> itemUnitsList = {"шт", "г", "м", "см", "л"};
    QVector<QString> itemUnitsIdsList = {"1", "2", "3", "4", "5"};
    QList<QStandardItem*> *itemUnitsSelector;
    for (int i=0; i<itemUnitsList.size(); i++)
    {
        itemUnitsSelector = new QList<QStandardItem*>();
        *itemUnitsSelector << new QStandardItem(itemUnitsList.at(i)) << new QStandardItem(itemUnitsIdsList.at(i));
        itemUnitsModel->appendRow(*itemUnitsSelector);
    }
    itemUnitsModel->setObjectName("itemUnitsModel");
    itemUnitsModel->setHorizontalHeaderLabels({"name", "id"});

    QVector<QString> storeItemsIntReserveStatesList = {QObject::tr("Ожидание"), QObject::tr("Товар выдан инженеру"), QObject::tr("Товар установлен"), QObject::tr("Товар установлен, устройство выдано"), QObject::tr("Архив"), QObject::tr("Отклонена")};
    for (int i=0; i<storeItemsIntReserveStatesList.size(); i++)
    {
        QList<QStandardItem*> *storeItemsIntReserveStateSelector = new QList<QStandardItem*>();
        *storeItemsIntReserveStateSelector << new QStandardItem(storeItemsIntReserveStatesList.at(i)) << new QStandardItem(QString::number(i));
        storeItemsIntReserveStatesModel->appendRow(*storeItemsIntReserveStateSelector);
    }
    storeItemsIntReserveStatesModel->setObjectName("storeItemsIntReserveStatesModel");
    storeItemsIntReserveStatesModel->setHorizontalHeaderLabels({"name", "id"});

#ifdef QT_DEBUG
    initClients4Test();
#endif
}

void initUserDbData()
{
    userDbDataModel->setQuery(QUERY_SEL_USER_DATA(QSqlDatabase::database("connMain").userName()), QSqlDatabase::database("connMain"));
    userDbDataModel->setObjectName("userDbDataModel");

    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные пользователя могут быть дополнены (например, кодом текущего офиса, если у пользователя есть право выбора офиса при входе)
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД (например, ширины столбцов таблиц, которые могут иметь разные значения в случае если пользователь работает на разных ПК).
    for (int i = 0; i < userDbDataModel->record(0).count(); i++)
    {
        userDbData->insert(userDbDataModel->record(0).fieldName(i), userDbDataModel->record(0).value(i));
    }
}

void initPermissions()
{
    QSqlQuery *queryPermissions = new QSqlQuery(QSqlDatabase::database("connMain"));

    queryPermissions->exec(QUERY_SEL_PERMISSIONS(userDbData->value("roles").toString()));
    while (queryPermissions->next())
    {
        permissions->insert(queryPermissions->value(0).toString(), 1);    // разрешённые пользователю действия хранятся в объекте QMap, не перечисленные действия не разрешены
    }

    delete queryPermissions;
}

void initCompanies()    // Список компаний.
{
    companiesModel->setQuery(QUERY_SEL_COMPANIES, QSqlDatabase::database("connMain"));
    companiesModel->setObjectName("companiesModel");
}

void initOffices()      // Список офисов
{
    officesModel->setQuery(QUERY_SEL_OFFICES(1), QSqlDatabase::database("connMain"));
    officesModel->setObjectName("officesModel");
}

void initSystemObjects()
{
    userActivityLog = new SUserActivityModel();
//    enum ClientBinaryOption{Company = 1, Regular = 2, Broker = 4, IgnoreCalls = 8, PreferCashless = 16, TakeLong = 32, Supplier = 64, SaleOrReturn = 128, BalanceEnabled = 256, Bad = 512, Archived = 1024};
    clientBinaryProperties = new SStandardItemModel();
    QVector<QString> NamesList = {QObject::tr("Юридическое лицо", "company"), QObject::tr("Постоянный клиент", "regular"),
                                  QObject::tr("Посредник", "broker (middleman)"), QObject::tr("Игнорирует звонки", "ignores calls"),
                                  QObject::tr("Предпочитает безнал", "prefer cashless"), QObject::tr("Не забирает технику в срок", "take out not in time"),
                                  QObject::tr("Является поставщиком", "supplier"), QObject::tr("Даёт товар под реализацию", "sale or return"),
                                  QObject::tr("Баланс", "client balance"), QObject::tr("Проблемный", "problematic client"), QObject::tr("Архивный", "archived")};
    QVector<QString> NamesListShort = {QObject::tr("юр.", "company (short)"), QObject::tr("пост.", "regular (short)"),
                                  QObject::tr("поср.", "broker (middleman) (short)"), QObject::tr("игн. зв.", "ignores calls (short)"),
                                  QObject::tr("безнал", "prefer cashless (short)"), QObject::tr("долго забир.", "take out not in time (short)"),
                                  QObject::tr("поставщ.", "supplier (short)"), QObject::tr("реализ.", "sale or return (short)"),
                                  QObject::tr("бал.", "client balance (short)"), QObject::tr("проблемный", "problematic client (short)"), QObject::tr("арх.", "archived (short)")};
    QVector<int> bitfieldsList = {SClientModel::Company, SClientModel::Regular,
                                  SClientModel::Broker, SClientModel::IgnoreCalls,
                                  SClientModel::PreferCashless, SClientModel::TakeLong,
                                  SClientModel::Supplier, SClientModel::SaleOrReturn,
                                  SClientModel::BalanceEnabled, SClientModel::Bad, SClientModel::Archived};
    QVector<QString> DBFieldsList = {"type", "is_regular",
                                     "is_agent", "ignore_calls",
                                     "prefer_cashless", "take_long",
                                     "is_dealer", "is_realizator",
                                     "balance_enable", "is_bad", "state"};
    QList<QStandardItem*> *selector;
    for (int i=0; i<NamesList.size(); i++)
    {
        selector = new QList<QStandardItem*>();
        *selector << new QStandardItem(NamesList.at(i)) << new QStandardItem(QString::number(bitfieldsList.at(i))) << new QStandardItem(DBFieldsList.at(i)) << new QStandardItem(NamesListShort.at(i));
        clientBinaryProperties->appendRow(*selector);
    }
    clientBinaryProperties->setObjectName("clientBinaryProperties");
    clientBinaryProperties->setHorizontalHeaderLabels({"name", "bitfield", "dbField, shortName"});
}
