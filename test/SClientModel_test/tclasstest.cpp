#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include "../../models/sclientmodel.h"
#include <SLocalSettings>
#include <SPhoneModel>
#include <SPhonesModel>
#include <SLogRecordModel>

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();
private:
    std::unique_ptr<TDebugStuff> debugStuff;
    QMap<QString, QVariant> testData;
    bool emulateBalanceFail = 0;
    void initAuxiliaryModels();
    void prepareDb();
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_loadModel();
    void test_modelCleaning();
    void test_demo();
    void test_fullName();
    void test_addPhone();
    void test_optionsList();
    void test_balanceOnOff();
    void test_updateBalance();
    void test_updateBalanceFail();
    void test_reportFields();
    void test_noPermissions();  // Этот тест должен быть последним
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

void TClassTest::initAuxiliaryModels()
{
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

    clientAdTypesList->setQuery(QUERY_SEL_CLIENT_AD_TYPES, QSqlDatabase::database(TdConn::main()));
    clientAdTypesList->setObjectName("clientAdTypesList");

    selector = new QList<QStandardItem*>();
    *selector << new QStandardItem("Retail") << new QStandardItem("1") << new QStandardItem("`price2`");
    priceColModel->appendRow(*selector);
}

void TClassTest::prepareDb()
{
    QFile file;
    QByteArray fileContent;
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    query->exec("SHOW TRIGGERS FROM `snap_app_test`;");
    while(query->next())
    {
        if(query->value(0).toString().compare("balance_fuckuper") == 0)
        {
            emulateBalanceFail = 1;
            break;
        }
    }
}

void TClassTest::initTestCase()
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF8");
//    QTextCodec::setCodecForLocale(codec);
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("SClientModel_test", testData);
}

void TClassTest::cleanupTestCase()
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    query->exec("ROLLBACK;");
    query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::session()));
    query->exec("ROLLBACK;");

}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database(TdConn::main()).isOpen() == 1);
    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
    initAuxiliaryModels();
    prepareDb();
}

void TClassTest::test_newModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::Client);
    QVERIFY(cut->m_phones != nullptr);
    QCOMPARE(cut->phones()->allPhones().count(), 0);
    QVERIFY(cut->balanceLog == nullptr);

}

void TClassTest::test_loadModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();

    cut->load(testData.value("id1").toInt());
    QCOMPARE(cut->cache.count(), 0);
    QVERIFY(cut->isModelLoaded() == 1);
    QCOMPARE(cut->id(), testData.value("id1").toInt());
    QCOMPARE(cut->isBalanceEnabled(), testData.value("isBalanceEnabled1").toBool());
    QCOMPARE(cut->phones()->primaryStr(), testData.value("phone1").toString());
    if(cut->isBalanceEnabled())
        QVERIFY(cut->balanceLog != nullptr);
    else
        QVERIFY(cut->balanceLog == nullptr);
    QCOMPARE(cut->printBOQDefaultState(), testData.value("printBOQDefaultState1").toBool());
}

void TClassTest::test_modelCleaning()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();

    cut->load(testData.value("id1").toInt());
    cut->clear();
    QCOMPARE(cut->cache.count(), 0);
    int loadedValuesCount = 0;
    for(int i = 0; i < cut->m_record->count(); i++)
    {
        if(cut->m_record->value(i).isValid())
            loadedValuesCount++;
    }
    QCOMPARE(loadedValuesCount, 0);
}

void TClassTest::test_demo()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();

    cut->initDemo();
    QCOMPARE(cut->cache.count(), 0);
    QVERIFY(cut->isModelLoaded() == 1);
    int demoValuesCount = 0;
    for(int i = 0; i < cut->m_record->count(); i++)
    {
        if(cut->m_record->value(i).isValid())
            demoValuesCount++;
    }
    QCOMPARE(demoValuesCount, 28);
}

void TClassTest::test_fullName()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();

    cut->set_firstName("Михаил");
    cut->set_patronymicName("Васильевич");
    cut->set_lastName("Ломоносов");
    QCOMPARE(cut->fullLongName(), "Ломоносов Михаил Васильевич");
    QCOMPARE(cut->fullShortName(), "Ломоносов М. В.");

    cut->clear();
    cut->set_firstName("Бауыржан");
    cut->set_lastName("Момышулы");
    QCOMPARE(cut->fullLongName(), "Момышулы Бауыржан");
    QCOMPARE(cut->fullShortName(), "Момышулы Б.");

    cut->clear();
    cut->set_isCompany(true);
    cut->set_firstName("НИИхиммаш");
    QCOMPARE(cut->fullLongName(), "НИИхиммаш");
    QCOMPARE(cut->fullShortName(), "НИИхиммаш");
    cut->set_urName("НПО «Энергия»");
    QCOMPARE(cut->fullLongName(), "НПО «Энергия»");
    QCOMPARE(cut->fullShortName(), "НПО «Энергия»");
}

void TClassTest::test_addPhone()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();
    QCOMPARE(cut->phones()->isUpdated(), 0);
    cut->addPhone("+7 111 111 11 11", 0, 0);
    QCOMPARE(cut->phones()->phonesList().count(), 1);
    QCOMPARE(cut->phones()->primaryStr(), "+7 111 111 11 11");
    QCOMPARE(cut->phones()->isUpdated(), 1);
}

void TClassTest::test_optionsList()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();
    cut->set_isBad(1);
    cut->set_isTakeLong(1);
    cut->set_isIgnoreCalls(1);
    QCOMPARE(cut->optionsList(SClientModel::OptionsOutputForm::Standard), QStringList({"Игнорирует звонки","Не забирает технику в срок","Проблемный"}));
    QCOMPARE(cut->optionsList(SClientModel::OptionsOutputForm::Short), QStringList({"игн. зв.","долго забир.","проблемный"}));
}

void TClassTest::test_balanceOnOff()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));

    auto cut = std::make_unique<SClientModel>();
    QVERIFY(cut->isBalanceEnabled() == 0);
    cut->set_isBalanceEnabled(true);
    QVERIFY(cut->isBalanceEnabled() == 1);    // в новой модели можно включить напрямую
    QVERIFY(cut->balanceLog == nullptr);      // объект не создаётся

    cut = std::make_unique<SClientModel>();
    query->exec("BEGIN;");
    cut->setPrimaryKey(testData.value("id2").toInt());
    QVERIFY(cut->isBalanceEnabled() == 0);
    cut->set_isBalanceEnabled(true);
    QVERIFY(cut->isBalanceEnabled() == 0);    // если primary key валидный, то баланс можно включить только вызвав метод setBalanceEnabled()
    cut->setBalanceEnabled(true);
    QVERIFY(cut->isBalanceEnabled() == 1);
    QVERIFY(cut->balanceLog != nullptr);      // объект создаётся
    QCOMPARE(cut->i_logRecord->text(), tr("Включен баланс клиента"));
    query->exec("ROLLBACK;");

    cut = std::make_unique<SClientModel>();
    query->exec("BEGIN;");
    cut->setPrimaryKey(testData.value("id1").toInt());
    cut->load();
    bool tmp = (cut->balance() != 0);   // баланс не может быть выключен, если он не равен нулю
    QVERIFY(cut->isBalanceEnabled() == testData.value("isBalanceEnabled1").toBool());
    cut->setBalanceEnabled(false);
    QVERIFY(cut->isBalanceEnabled() == tmp);
    if(tmp)
        QVERIFY(cut->balanceLog != nullptr);      // объект не удалится
    else
        QVERIFY(cut->balanceLog == nullptr);      // объект удалится
    query->exec("ROLLBACK;");
}

void TClassTest::test_updateBalance()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));

    auto cut = std::make_unique<SClientModel>();
    QVERIFY(cut->updateBalance(0, "") == 1);

    // при попытке изменить баланс нового клиента должно генерироваться исключение (в основной программе будет выводиться всплывающее сообщение)
    int t = 0;
    try{
        cut->updateBalance(1, "");
        t = 1;
    }
    catch(Global::ThrowType){
        t = 2;
    }

    cut = std::make_unique<SClientModel>();
    query->exec("BEGIN;");
    cut->setPrimaryKey(testData.value("id1").toInt());
    cut->load();
    double oldValue = cut->balance();
    double amount = 1000;
    QString reason = "Поступление средств с зачислением на баланс клиента";
    t = 0;
    try{
        cut->updateBalance(amount, reason, SBalanceLogRecordModel::Repair, 1);
        QCOMPARE(cut->balanceLog->reason(), reason);
        QCOMPARE(cut->balanceLog->i_logRecord->text(), reason);
        QCOMPARE(cut->balanceLog->i_logRecord->repair(), 1);
        QCOMPARE(cut->balanceLog->i_logRecord->document(), 0);
        QCOMPARE(cut->balanceLog->i_logRecord->cashOrder(), 0);
        QCOMPARE(cut->balanceLog->i_logRecord->cacheRef(SLogRecordModel::C_id)->state(), SSingleRowModelBase::ModifiedField::Executed);
        QCOMPARE(cut->balanceLog->cacheRef(SClientModel::C_id)->state(), SSingleRowModelBase::ModifiedField::Executed);
        QCOMPARE(cut->balance(), oldValue + amount);

        cut->updateBalance(amount, reason, SBalanceLogRecordModel::Document, 1);
        QCOMPARE(cut->balanceLog->i_logRecord->repair(), 0);
        QCOMPARE(cut->balanceLog->i_logRecord->document(), 1);
        QCOMPARE(cut->balanceLog->i_logRecord->cashOrder(), 0);
        QCOMPARE(cut->balance(), oldValue + amount*2);

        cut->updateBalance(amount, reason, SBalanceLogRecordModel::CashOrder, 1);
        QCOMPARE(cut->balanceLog->i_logRecord->repair(), 0);
        QCOMPARE(cut->balanceLog->i_logRecord->document(), 0);
        QCOMPARE(cut->balanceLog->i_logRecord->cashOrder(), 1);
        QCOMPARE(cut->balance(), oldValue + amount*3);

        t = 1;
    }
    catch(Global::ThrowType){
        t = 2;
    }
    query->exec("ROLLBACK;");
    QCOMPARE(t, 1);
    cut->setState(SClientModel::C_balance, SSingleRowModelBase::ModifiedField::Failed);
    QCOMPARE(cut->cache[SClientModel::C_balance]->state(), SSingleRowModelBase::ModifiedField::Failed);
    QCOMPARE(cut->balance(), oldValue);
}

/* Тест использует специальный триггер balance_fuckuper, изменяющий значение `balance`.`summ` при выполнении INSERT.
 * Перед выполенением теста необходимо добавить этот триггер в тестовую базу используя сниппет 'sql/balance_fuckuper.sql'.
*/
void TClassTest::test_updateBalanceFail()
{
//    QSKIP("");
    if(!emulateBalanceFail)
        QSKIP("Trigger 'balance_fuckuper' not detected. Please apply the 'balance_fuckuper.sql' snippet to the test database first.");

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));

    auto cut = std::make_unique<SClientModel>();
    query->exec("BEGIN;");
    query->exec("UPDATE `global_user_variables` SET `value`='1' WHERE `name`='test_trig_balance_fuckuper';");
    cut->setPrimaryKey(testData.value("id1").toInt());
    cut->load();
    int t = 0;
    try{
        cut->updateBalance(1000, "reason");
        t = 1;
    }
    catch(Global::ThrowType){
        t = 2;
    }
    query->exec("ROLLBACK;");
    QCOMPARE(t, 2);
}

/* Проверка сооответствия названий и кол-ва полей для отчетов.
 * Желательно, чтобы при изменении модели поля не изменялись, чтобы не переделывать отчеты. Этот тест выявит различия.
*/
void TClassTest::test_reportFields()
{
//    QSKIP("");
    auto cut = std::make_unique<SClientModel>();
    cut->setPrimaryKey(testData.value("id1").toInt());
    cut->load();
    LimeReport::CallbackInfo repInfo;
    QVariant repData;

    repInfo.dataType = LimeReport::CallbackInfo::ColumnCount;
    cut->reportCallbackData(repInfo, repData);
    int columnCount = repData.toInt();
    // Список всех полей, доступных в отчете
    QStringList fields = {"address", "adTypeStr", "balance", "birthday", "email", "web_password", "photo", "KPP", "urName", "postIndex", "firstName", "isBalanceEnabled", "priceColumn", "lastName", "purchases", "memorial", "fullLongName", "notes", "repairs", "isPreferCashless", "shortName", "isIgnoreCalls", "OGRN", "site", "fullShortName", "isGivesItemsForSale", "isCompany", "patronymicName", "passportIssuedDate", "isBroker", "passportNum", "isSupplier", "passportIssuedBy", "isTakeLong", "state", "isRegular", "id", "INN", "isArchived", "isBad"};

    for(int i = 0; i < columnCount; i++)
    {
        QVERIFY(fields.count() != 0);
        repInfo.dataType = LimeReport::CallbackInfo::ColumnHeaderData;
        repInfo.index = i;
        cut->reportCallbackData(repInfo, repData);
        fields.removeOne(repData.toString());
    }

    QVERIFY(fields.count() == 0);
}

// Этот тест должен вывзываться последним, т. к. в  нём изменяются значения полей класса permissions
void TClassTest::test_noPermissions()
{
//    QSKIP("");

    permissions->viewClients = 0;
    permissions->viewClientPassportData = 0;

    auto cut = std::make_unique<SClientModel>();
    cut->setPrimaryKey(testData.value("id1").toInt());
    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    cut->load();
    QCOMPARE(cut->firstName(), tr("no permissions"));
    QCOMPARE(cut->lastName(), tr("no permissions"));
    QCOMPARE(cut->patronymicName(), tr("no permissions"));
    QCOMPARE(cut->address(), tr("no permissions"));
    QCOMPARE(cut->email(), tr("no permissions"));
    QCOMPARE(cut->passportNum(), tr("no permissions"));
    QCOMPARE(cut->passportIssuedDate(), QDate());
    QCOMPARE(cut->passportIssuedBy(), tr("no permissions"));
    QCOMPARE(cut->INN(), tr("no permissions"));
    QCOMPARE(cut->KPP(), tr("no permissions"));
    QCOMPARE(cut->OGRN(), tr("no permissions"));
    QCOMPARE(cut->phones()->primary()->phone(), tr("no permissions"));
    QCOMPARE(cut->phones()->primary()->phoneClean(), tr("no permissions"));
    QCOMPARE(cut->phones()->primary()->note(), tr("no permissions"));
}

QTEST_MAIN(TClassTest)

#include "tclasstest.moc"
