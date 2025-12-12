#include <QtTest>
#include <QStandardItem>
#include <QThread>
#include "../tdebugstuff.h"
#include <ProjectGlobals>
#include <SCartridgeRepairModel>
#include <SRepairStatusLog>
#include "../../models/srepairmodel.h"
#include <SLocalSettings>
#include <SLogRecordModel>

std::shared_ptr<TDebugStuff> debugStuff;

class ThreadPerUser : public QObject
{
    Q_OBJECT
public:
    explicit ThreadPerUser(int id = 0)
        : m_id(id){};
    ~ThreadPerUser(){
        auto db = QSqlDatabase::database(connNameForThread());
        db.close();
        QSqlDatabase::removeDatabase(connNameForThread());
    };
public slots:
    void init()
    {
        userDbData->set_id(1);

        auto settings = debugStuff->debugOptions;
        settings->beginGroup("debugLogin");
        auto db = QSqlDatabase::addDatabase("QMYSQL", connNameForThread());
        db.setUserName(settings->value("user").toString());
        db.setPassword(settings->value("password").toString());
        db.setHostName(settings->value("host").toString());
        db.setPort(settings->value("port").toInt());
        db.setDatabaseName(settings->value("database").toString());
        settings->endGroup();

        overrideCreds(db);

        if (!db.open()) {
            qWarning() << connNameForThread() << "open failed:" << db.lastError();
            return;
        }
    };
    bool isConnOpen(){
        return QSqlDatabase::database(connNameForThread()).isOpen();
    };
    void initCUT(const int id){
        m_cut = std::make_shared<SRepairModel>();
        m_cut->setDatabase(QSqlDatabase::database(connNameForThread()));
        m_cut->setPrimaryKey(id);
        loadCUT();
    };
    void loadCUT(){
        m_cut->load();
    };
    void setCUTLock(const bool state){
        m_cut->lock(state);
    };
    void setCUTLock(const QVariant &userId, const QDateTime &time = QDateTime()){
        m_cut->setData(SRepairModel::C_userLock, userId);
        m_cut->setData(SRepairModel::C_lockDateTime, time);
        m_cut->commit();
    };
    bool isCUTLocked(){
        auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(connNameForThread()));
        bool ret = m_cut->isLock();
        return ret;
    };
protected:
    QString m_conn;
    virtual void overrideCreds(QSqlDatabase&){};
    QString connNameForThread(){
        return TdConn::session(); // QString("test_lock_conn_%1").arg(m_id)
    };
    QSqlDatabase connForThread(){
        return QSqlDatabase::database(connNameForThread());
    };
private:
    int m_id;
    std::shared_ptr<SRepairModel> m_cut;
};

class FirstUserThread : public ThreadPerUser
{
public:
    explicit FirstUserThread() : ThreadPerUser(1){};
};

class SecondUserThread : public ThreadPerUser
{
public:
    explicit SecondUserThread(const QString& login, const QString& password, const int userId) :
        ThreadPerUser(2),
        m_login(login),
        m_password(password),
        m_uid(userId)
    {
    };
    void overrideCreds(QSqlDatabase &db) override
    {
        db.setUserName(m_login);
        db.setPassword(m_password);
        userDbData->set_id(m_uid);
    };
private:
    QString m_login;
    QString m_password;
    int m_uid;
};

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();
private:
    QMap<QString, QVariant> testData;
    void initAuxiliaryModels();
    QDateTime currentDateTime();
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_loadModel();
    void test_loadCartridgeRepairModel();
    void test_stateChange();
    void test_informedStateChange();
    void test_boxChange();
    void test_paymentsSumm();
    void test_addPrepay();
    void test_usersChange();
    void test_diagChange();
    void test_timestamps();
    void test_lock();
    void test_actualCosts();
    void test_modelCleaning();
    void test_demo();
    void test_reportFields();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

void TClassTest::initAuxiliaryModels()
{
}

QDateTime TClassTest::currentDateTime()
{
    QDateTime now = QDateTime::currentDateTime();
    QTime time = QTime::currentTime();
    now.setTime(QTime(time.hour(), time.minute(), time.second()));  // Обнуляем миллисекунды (они не записываются в БД)

    return now;
}

void TClassTest::initTestCase()
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF8");
//    QTextCodec::setCodecForLocale(codec);
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_shared<TDebugStuff>();
    debugStuff->readTestData("SRepairModel_test", testData);
}

void TClassTest::cleanupTestCase()
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("ROLLBACK;");
    query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::session()));
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("ROLLBACK;");

}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database(TdConn::main()).isOpen() == 1);
    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
    initAuxiliaryModels();
}

void TClassTest::test_newModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::Repair);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->set_classId(testData.value("class").toInt());
    cut->set_vendorId(testData.value("vendor").toInt());
    cut->set_deviceId(testData.value("model").toInt());
    cut->set_title(testData.value("title").toString());
    cut->set_serialNumber(testData.value("serial").toString());
    cut->set_managerId(userDbData->id());
    cut->set_currentManagerId(userDbData->id());
    cut->set_fault(testData.value("fault").toString());
    cut->set_complect(testData.value("complect").toString());
    cut->set_look(testData.value("look").toString());
    cut->set_extNotes(testData.value("extNotes").toString());
    cut->set_clientId(testData.value("client").toInt());
    cut->set_preAgreedAmount(2000);
    QCOMPARE(cut->i_logTexts.count(), 2);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Предварительная стоимость ремонта: %1").arg(sysLocale.toCurrencyString(2000))), 1);
    cut->commit();
    cut->commit(); // второй вызов не должен генерировать запросов к БД

    QCOMPARE(cut->cache[SRepairModel::C_id]->state(), SSingleRowModelBase::ModifiedField::Executed);

    query->exec("ROLLBACK;");
}

void TClassTest::test_loadModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();
    int signalsCounter = 0;
    auto sl = [&](){signalsCounter++;};
    connect(cut.get(), &SRepairModel::modelUpdated, sl);

    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->cache.count(), 0);
    QVERIFY(cut->isModelLoaded() == 1);
    QVERIFY(cut->isNew() == 0);
    QCOMPARE(cut->id(), testData.value("id").toInt());
    QCOMPARE(signalsCounter, 1);
}

void TClassTest::test_loadCartridgeRepairModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    cut->load(testData.value("cartridgeRepair_workshop_id").toInt());
    QVERIFY(cut->isModelLoaded() == 1);
    QCOMPARE(cut->cartridgeId(), testData.value("cartridgeRepair_c_workshop_id").toInt());
    QVERIFY(cut->m_cartridgeRepair != nullptr);
    QVERIFY(cut->m_cartridgeRepair->isModelLoaded() == 1);
    QCOMPARE(cut->m_cartridgeRepair->repairId(), cut->id());
}

void TClassTest::test_stateChange()
{
//    QSKIP("");
    QDateTime now = currentDateTime();
    auto cut = std::make_unique<SRepairModel>();

    cut->load(testData.value("id").toInt());
    cut->set_stateId(1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Статус заказа изменён на \"%1\"").arg(comSettings->repairStatusesVariantCopy()[1].Name)), 1);
    QCOMPARE(cut->m_repairStatusLog->repair(), testData.value("id").toInt());
    QCOMPARE(cut->m_repairStatusLog->status(), 1);
    QCOMPARE(cut->m_repairStatusLog->manager(), cut->currentManagerId());
    QCOMPARE(cut->m_repairStatusLog->engineer(), cut->engineerId());
    QVERIFY(std::abs(cut->lastStatusChanged().secsTo(now)) < 1);
    QVERIFY(std::abs(cut->lastSave().secsTo(now)) < 1);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    query->exec("ROLLBACK;");
}

void TClassTest::test_informedStateChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    cut->load(testData.value("id").toInt());
    cut->set_informedStatusId(testData.value("informStatus").toInt());
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Статус информирования клиента изменён на \"%1\"").arg(notifyStatusesModel->getDisplayRole(testData.value("informStatus").toInt()))), 1);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    query->exec("ROLLBACK;");
}

void TClassTest::test_boxChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();


    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    query->exec(QString("UPDATE `workshop` SET `box` = NULL WHERE `id` = %1;").arg(testData.value("id").toInt()));
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->boxIndex(), -1);
    cut->setBoxIndex(1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Заказаз-наряд %1 помещён в ячейку \"%2\"").arg(testData.value("id").toInt()).arg(repairBoxesModel->getDisplayRole(1))), 1);
    cut->commit();

    cut->setBoxIndex(2);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Заказаз-наряд %1 перемещён в ячейку \"%2\"").arg(testData.value("id").toInt()).arg(repairBoxesModel->getDisplayRole(2))), 1);
    cut->commit();

    cut->setBoxIndex(0);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Заказаз-наряд %1 изъят из ячейки \"%2\"").arg(testData.value("id").toInt()).arg(repairBoxesModel->getDisplayRole(2))), 1);
    cut->commit();

    cut->setBoxIndex(0);
    QCOMPARE(cut->i_logTexts.count(), 0);
    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_paymentsSumm()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->prepaidSumm(), 0);
    query->exec(QString("INSERT INTO `cash_orders` (`created`, `type`, `summa`, `summa_str`, `invoice`, `client`, `to_user`, `user`, `company`, `office`, `notes`, `repair`, `document`, `img`, `payment_system`, `card_fee`, `is_backdate`, `card_info`, `customer_email`, `fdn`, `payment_item_sign`)"
                        " VALUES ('2025-08-22 06:44:43', 12, 1000.0000, 'тысяча рублей, 00 копеек', NULL, %1, NULL, %2, 1, 1, 'Предоплата за ремонт (test_paymentsSumm)', %3, NULL, NULL, 0, 0.0000, 0, NULL, NULL, NULL, NULL);")
                .arg(testData.value("client").toInt())
                .arg(userDbData->id())
                .arg(testData.value("id").toInt()));
    auto amount = cut->paymentsAmount(SCashRegisterModel::PaymentType::RecptPrepayRepair);
    QCOMPARE(amount, 1000.00);
    query->exec("ROLLBACK;");
}

void TClassTest::test_addPrepay()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->setDatabase(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->prepaidSumm(), 0);
    cut->addPrepay(1000, __func__);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Предоплата в размере %1 (%2)").arg(sysLocale.toCurrencyString(1000)).arg(__func__)), 1);
    cut->commit();
    cut->commit(); // второй вызов не должен генерировать запросов к БД
    QCOMPARE(cut->prepaidSumm(), 1000.00);
    QCOMPARE(cut->isPrepaid(), 1);

    cut->addPrepay(-1000);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Возврат предоплаты в размере %1").arg(sysLocale.toCurrencyString(1000))), 1);
    cut->commit();
    QCOMPARE(cut->prepaidSumm(), 0.00);
    QCOMPARE(cut->isPrepaid(), 0);

    query->exec("ROLLBACK;");

}

void TClassTest::test_usersChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->setDatabase(db);
    cut->load(testData.value("id").toInt());
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->set_currentManagerId(1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Менеджером ремонта назначен %1").arg(allUsersModel->value(1, "id", "username").toString())), 1);
    cut->commit();
    cut->set_engineerId(testData.value("lockUserId").toInt());
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Инженером назначен %1").arg(allUsersModel->value(testData.value("lockUserId").toInt(), "id", "username").toString())), 1);
    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_diagChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->setDatabase(db);
    cut->load(testData.value("id").toInt());
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->set_diagnosticResult(tr("%1").arg(__func__));
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Результат диагностики изменён на \"%1\"").arg(__func__)), 1);
    cut->commit();

    cut->set_repairCost(3000);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Стоимость ремонта в результатах диагностики изменёна на %1").arg(sysLocale.toCurrencyString(3000))), 1);
    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_timestamps()
{
//    QSKIP("");
    QDateTime now = currentDateTime();
    QDateTime nowUtc = now.toUTC();
    auto cut = std::make_unique<SRepairModel>();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->setDatabase(db);
    cut->load(testData.value("id").toInt());
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->set_created(now);
    cut->set_issued(now);
    cut->set_lastSave(now);
    cut->set_lastStatusChanged(now);
    cut->set_lockDateTime(now);

    // до и после коммита, а также после перезагрузки модели методы должны отдавать время с локальным часовым поясом
    QCOMPARE(cut->created(), now);
    QCOMPARE(cut->createdUtc(), nowUtc);
    QCOMPARE(cut->createdStr(), now.toString("dd.MM.yyyy hh:mm:ss"));
    QCOMPARE(cut->issued(), now);
    QCOMPARE(cut->outDateTime(), now.toString("dd.MM.yyyy hh:mm:ss"));
    QCOMPARE(cut->lastSave(), now);
    QCOMPARE(cut->lastStatusChanged(), now);
    QCOMPARE(cut->lockDateTime(), now);
    cut->commit();
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->created(), now);
    QCOMPARE(cut->createdUtc(), nowUtc);
    QCOMPARE(cut->createdStr(), now.toString("dd.MM.yyyy hh:mm:ss"));
    QCOMPARE(cut->issued(), now);
    QCOMPARE(cut->outDateTime(), now.toString("dd.MM.yyyy hh:mm:ss"));
    QCOMPARE(cut->lastSave(), now);
    QCOMPARE(cut->lastStatusChanged(), now);
    QCOMPARE(cut->lockDateTime(), now);

    query->exec("ROLLBACK;");
}

void TClassTest::test_lock()
{
//    QSKIP("");
    int uidBackup = userDbData->id();
    QDateTime now = currentDateTime();
    QDateTime nowUtc = now.toUTC();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec(QString("UPDATE `workshop` SET `user_lock` = NULL, `lock_datetime` = NULL WHERE `id` = %1;").arg(testData.value("id").toInt()));

    // Проверка привилегии PROCESS
    auto checkGrants = [&](){
        bool ret = 0;
        QString grants;
        int end;
        while(query->next() && !ret)
        {
            grants = query->value(0).toString();
            grants.replace("GRANT ", "");
            end = grants.indexOf(" ON *.*");
            if(end < 0)
                continue;

            grants = grants.left(end).replace(", ", ",");
            QStringList grantsLst = grants.split(',');
            ret = grantsLst.contains("PROCESS") || grantsLst.contains("ALL PRIVILEGES");
        }
        return ret;
    };
    query->exec("SHOW GRANTS FOR 'admin';");
    QVERIFY(checkGrants() == 1);
    query->exec("SHOW GRANTS FOR 'test';");
    QVERIFY2(checkGrants() == 1, "Для теста блокировки необходимо, чтобы пользователь 'test'@'%' обладал привилегией PROCESS");

    QThread* qtd1 = new QThread();
    FirstUserThread *thread1 = new FirstUserThread();
    thread1->moveToThread(qtd1);
    QObject::connect(qtd1, &QThread::started, thread1, &FirstUserThread::init);
    qtd1->start();

    bool isOpen = 0;
    QMetaObject::invokeMethod(thread1, "isConnOpen", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isOpen));
    QVERIFY(isOpen == 1);

    bool isLocked = 0;
    QMetaObject::invokeMethod(thread1, "initCUT", Qt::BlockingQueuedConnection, Q_ARG(int, testData.value("id").toInt()));
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);

    // если поле lock_datetime по какой-то причине не равно NULL, а поле lock_user равно NULL, то карточка не считается заблокированной
    QMetaObject::invokeMethod(thread1, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(QVariant, QVariant()), Q_ARG(QDateTime, now));
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);

    // если поле lock_user не равно NULL, а поле lock_datetime по какой-то причине равно NULL, то карточка также не считается заблокированной
    QMetaObject::invokeMethod(thread1, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(QVariant, 1), Q_ARG(QDateTime, QDateTime()));
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);

    QMetaObject::invokeMethod(thread1, "loadCUT", Qt::BlockingQueuedConnection);    // перезагрузка модели, чтобы очистить кэш
    QMetaObject::invokeMethod(thread1, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(QVariant, testData.value("lockUserId").toInt()), Q_ARG(QDateTime, now));
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);    // карта не заблокирована, т. к. другой пользователь оффлайн

    QVERIFY2(testData.value("lockUserPwd").toString().isEmpty() == false, "Не указан пароль пользователя 'test' в файле debug.ini");
    query->exec("# New database connection to simulate online status of the user who locked the repair is being established right now…");
    QThread* qtd2 = new QThread();
    SecondUserThread *thread2 = new SecondUserThread(
                                        testData.value("lockUsername").toString(),
                                        testData.value("lockUserPwd").toString(),
                                        testData.value("lockUserId").toInt());
    thread2->moveToThread(qtd2);
    QObject::connect(qtd2, &QThread::started, thread2, &SecondUserThread::init);
    qtd2->start();
    QMetaObject::invokeMethod(thread2, "isConnOpen", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isOpen));
    QVERIFY2(isOpen == 1, "Не удалось подключиться к БД пользователем 'test'");

    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 1);    // карта заблокирована, другой пользователь онлайн
    QMetaObject::invokeMethod(thread2, "initCUT", Qt::BlockingQueuedConnection, Q_ARG(int, testData.value("id").toInt()));
    QMetaObject::invokeMethod(thread2, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);    // карта не заблокирована, т. к. в `user_lock` записан id этого пользователя
    QMetaObject::invokeMethod(thread2, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(bool, false));
    QMetaObject::invokeMethod(thread1, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(bool, true));
    QMetaObject::invokeMethod(thread2, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 1);    // карта заблокирована первым пользователем

    QMetaObject::invokeMethod(thread1, "loadCUT", Qt::BlockingQueuedConnection);    // перезагрузка модели, чтобы очистить кэш
    QMetaObject::invokeMethod(thread1, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(bool, false));
    QMetaObject::invokeMethod(thread2, "loadCUT", Qt::BlockingQueuedConnection);
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);    // карта не заблокирована
    QMetaObject::invokeMethod(thread2, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);    // карта не заблокирована

    // Возможна очень редкая ситуация, когда карточка ремона открывается разными пользователями почти одновременно и запрос для
    // блокировки от первого пользователя мог еще не выполниться, в то время когда второй проверяет блокировку; при подключении
    // к удалённому серверу (ping timeout >5ms) и выполнению блокировки в транзакции (BEGIN ... COMMIT) этот тест выявлял
    // описанную проблему, однако при проверке с локальным сервером задержки очень маленькие и проблема не проявлялась;
    // теоретически, проблема может проявляться и при тормозах ПК; тест требует доработки, поэтому закомментирован.
/*  query->exec("# проверка блокировки при почти одновременном открытии карты ремонта разными пользователями");
    QMetaObject::invokeMethod(thread1, "loadCUT", Qt::QueuedConnection);    // при открытии карточки ремонта сперва загружаются данные таблицы `workshop`
    QThread::msleep(2); // задержка на ответ на запрос SELECT, обработку результата и загрузку данных о блокировке
    QMetaObject::invokeMethod(thread2, "loadCUT", Qt::QueuedConnection);    // а тем временем второй пользователь тоже открывает карточку ремонта
    QMetaObject::invokeMethod(thread1, "setCUTLock", Qt::QueuedConnection, Q_ARG(bool, true));
//    QThread::usleep(50); // аналогичная задержка для второго пользователя
    QMetaObject::invokeMethod(thread2, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    if(isLocked == 0)
        QMetaObject::invokeMethod(thread2, "setCUTLock", Qt::QueuedConnection, Q_ARG(bool, true));
    QMetaObject::invokeMethod(thread2, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 1);    // карта заблокирована
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);    // карта не заблокирована
*/

    // тайм-аут блокировки
    query->exec("# тайм-аут блокировки");
    QMetaObject::invokeMethod(thread2, "setCUTLock", Qt::BlockingQueuedConnection, Q_ARG(QVariant, userDbData->id()), Q_ARG(QDateTime, now.addSecs(-1000)));
    QMetaObject::invokeMethod(thread1, "isCUTLocked", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isLocked));
    QVERIFY(isLocked == 0);    // карта не заблокирована (таймаут), пользователь онлайн

    qtd1->terminate();
    qtd2->terminate();
}

void TClassTest::test_actualCosts()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->setDatabase(db);
    cut->setPrimaryKey(testData.value("id").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    query->exec(QString("DELETE FROM `store_int_reserve` WHERE `repair_id` = %1;").arg(testData.value("id").toInt()));
    query->exec(QString("DELETE FROM `works` WHERE `repair` = %1;").arg(testData.value("id").toInt()));
    query->exec(QString("INSERT INTO `works` (`user`, `repair`, `document_id`, `name`, `price`, `count`, `warranty`, `price_id`, `is_pay`, `added`, `type`, `pay_repair`, `pay_repair_quick`, `salary_summ`)"
                "VALUES (1, %1, NULL, 'Ремонт', 1000.0000, 1, 0, NULL, 0, NOW(), 0, NULL, NULL, NULL);")
                .arg(testData.value("id").toInt()));
    query->exec("SELECT LAST_INSERT_ID();");
    query->first();
    auto i = query->value(0).toInt();
    query->exec(QString("INSERT INTO `store_int_reserve` (`item_id`, `name`, `count`, `created`, `from_user`, `to_user`, `notes`, `state`, `repair_id`, `work_id`, `price`, `sn`, `warranty`, `r_lock`)"
                "VALUES (50, 'Конденсатор керамический 22мкФ*25В, X7R, 10%, 1206', 5, NOW(), 1, 1, 'автоматизированный тест test_actualCosts', 2, %1, %2, 19.0000, NULL, 31, 0);")
                .arg(testData.value("id").toInt())
                .arg(i));

    QCOMPARE(cut->realPartsCost(), 95);
    QCOMPARE(cut->realWorksCost(), 1000);

    query->exec("ROLLBACK;");
}

void TClassTest::test_modelCleaning()
{
//    QSKIP("");
//    auto cut = std::make_unique<SRepairModel>();

//    cut->load(testData.value("id").toInt());
//    cut->clear();
//    QCOMPARE(cut->cache.count(), 0);
//    int loadedValuesCount = 0;
//    for(int i = 0; i < cut->m_record->count(); i++)
//    {
//        if(cut->m_record->value(i).isValid())
//            loadedValuesCount++;
//    }
//    QCOMPARE(loadedValuesCount, 0);
}

void TClassTest::test_demo()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();

    cut->initDemo();
    QCOMPARE(cut->cache.count(), 0);
    QVERIFY(cut->isModelLoaded() == 1);
    int demoValuesCount = 0;
    for(int i = 0; i < cut->m_record->count(); i++)
    {
        if(cut->m_record->value(i).isValid())
            demoValuesCount++;
    }
    QCOMPARE(demoValuesCount, 57);
}

/* Проверка сооответствия названий и кол-ва полей для отчетов.
 * Желательно, чтобы при изменении модели поля не изменялись, чтобы не переделывать отчеты. Этот тест выявит различия.
*/
void TClassTest::test_reportFields()
{
//    QSKIP("");
    auto cut = std::make_unique<SRepairModel>();
    cut->setPrimaryKey(testData.value("id").toInt());
    cut->load();
    LimeReport::CallbackInfo repInfo;
    QVariant repData;

    repInfo.dataType = LimeReport::CallbackInfo::ColumnCount;
    cut->reportCallbackData(repInfo, repData);
    int columnCount = repData.toInt();
    // Список всех полей, доступных в отчете, до версии 0.0.0.304 (включительно)
    QStringList fields = {"barcode", "box", "canFormat", "realRepairCostWords", "realRepairCost", "diagnosticResult", "realWorksCost",
                          "class", "rejectReason", "clientId", "repairCost", "early", "partsCost", "serialNumber", "isDebt", "paymentSystem",
                          "outDateTime", "engineer", "color", "smsInform", "fault", "isPreAgreed", "isExpressRepair", "extEarly",
                          "preAgreedAmount", "outDate", "startOffice", "company", "extNotes", "isCardPayment", "prepaidOrder", "state",
                          "id", "prepaidSumm", "realPrepaidSumm", "currentManager", "termsControl", "inDate", "complect", "isPrepaid",
                          "isQuickRepair", "invoice", "manager", "office", "inDateTime", "vendor", "device", "prepaidType", "title",
                          "thirsPartySc", "printCheck", "isRepeat", "realPartsCost", "warrantyLabel", "issuedMsg", "isWarranty", "look", "warranty"};

    for(int i = 0; i < columnCount; i++)
    {
        QVERIFY(fields.count() != 0);
        repInfo.dataType = LimeReport::CallbackInfo::ColumnHeaderData;
        repInfo.index = i;
        cut->reportCallbackData(repInfo, repData);
        fields.removeOne(repData.toString());
    }

    // Эти поля исключил из отчетов в версии 0.0.0.305 - думаю, пользы от них нет.
    fields.removeOne("prepaidOrder");
    fields.removeOne("state");
    fields.removeOne("termsControl");
    fields.removeOne("invoice");

    if(fields.count() != 0)
    {
        qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(fields);
        QVERIFY(0);
    }
}

QTEST_MAIN(TClassTest)

#include "tclasstest.moc"
