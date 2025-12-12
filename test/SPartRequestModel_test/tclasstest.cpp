#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include <ProjectGlobals>
#include "../../modules/purchasemanager/srequest.h"
#include <SLocalSettings>
#include <SLogRecordModel>
#include <SEditableBaseModel>

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();
private:
    std::unique_ptr<TDebugStuff> debugStuff;
    QMap<QString, QVariant> testData;
    void initAuxiliaryModels();
    QDateTime currentDateTime();
    QList<int> managersList(const QVariant &csv);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_newModel_more_params();
    void test_statusChange();
    void test_priorityChange();
    void test_countChange();
    void test_planEndDateChange();
    void test_amountChange();
    void test_trackChange();
    void test_loadModel();
    void test_managersGetSet();
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

QList<int> TClassTest::managersList(const QVariant &csv)
{
    if(csv.type() == QVariant::String)
    {
        if(csv.toString().isEmpty())
            return QList<int>();
        else
            return QList<int>({csv.toInt()});   // если указано только одно число, то QSettings обработает его как строку
    }

    // если есть запятая, то значение обрабатывается как QStringList
    QList<int> list;
    foreach (auto v, csv.toStringList())
    {
        if(v.toInt() == 0)
            continue;

        list.append(v.toInt());
    }

    return list;
}

void TClassTest::initTestCase()
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF8");
//    QTextCodec::setCodecForLocale(codec);
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("SPartRequestModel_test", testData);

    partRequestStatesListModel = SPartRequest::statesList();
    partRequestPrioritiesListModel = SPartRequest::prioritiesList();
    partRequestManagersModel = SPartRequest::managersList();
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
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QCOMPARE(cut->i_logTexts.count(), 0);
    QCOMPARE(cut->m_managers->rowCount(), 0);
    QCOMPARE(cut->managers().count(), 5);

    cut->set_employee(userDbData->id());
    cut->set_state(SPartRequest::State::Created);
    cut->set_priority(SPartRequest::Priority::Regular);
    cut->set_name(testData.value("name").toString());
    cut->set_count(testData.value("qty").toInt());

    QCOMPARE(cut->i_logTexts.count(), 0);
    cut->commit();
    QCOMPARE(cut->i_logRecord->text().contains(" создана"), 1);
    QCOMPARE(cut->i_logRecord->text().contains("Приоритет: "), 1);
    QCOMPARE(cut->i_logRecord->text().contains("Deadline: "), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::RecordTypes::PartRequest);

    cut->commit(); // второй вызов не должен генерировать запросов к БД

    QCOMPARE(cut->cache[SPartRequest::C_id]->state(), SSingleRowModelBase::ModifiedField::Executed);

    query->exec("ROLLBACK;");
}

void TClassTest::test_newModel_more_params()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->set_employee(userDbData->id());
    cut->set_priority(SPartRequest::Priority::Regular);
    cut->set_name(testData.value("name").toString());
    cut->set_count(testData.value("qty").toInt());
    cut->set_state(SPartRequest::State::Created);
    cut->set_created(QDateTime::currentDateTime());
    cut->set_planEndDate(QDateTime::currentDateTime().addDays(7).date());
    cut->set_dealer(testData.value("dealer").toInt());
    cut->set_client(testData.value("client").toInt());
    cut->set_repair(testData.value("repair").toInt());
//    cut->set_notes("");

    QCOMPARE(cut->i_logTexts.count(), 0);
//    QCOMPARE(cut->i_logTexts.values().contains(), 1);
    cut->commit();
    QCOMPARE(cut->i_logRecord->text().contains(" создана"), 1);
    QCOMPARE(cut->i_logRecord->text().contains("Приоритет: "), 1);
    QCOMPARE(cut->i_logRecord->text().contains("Deadline: "), 1);

    QCOMPARE(cut->cache[SPartRequest::C_id]->state(), SSingleRowModelBase::ModifiedField::Executed);

    query->exec("ROLLBACK;");
}

void TClassTest::test_statusChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->set_employee(userDbData->id());
    cut->set_state(SPartRequest::State::Created);
    cut->set_priority(SPartRequest::Priority::Regular);
    cut->set_name(testData.value("name").toString());
    cut->set_count(testData.value("qty").toInt());

    // 0 будет передаваться если соответствующие поля на вкладке остаются не заполненными
    cut->set_dealer(0);
    cut->set_client(0);
    cut->set_repair(0);
    cut->commit();
    cut->SSingleRowJModel::load();    // после создания новой заявки производится загрузка, см. tabPartRequest::endCommit()
    QCOMPARE(cut->data(SPartRequest::C_client).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_client).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_dealer).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_dealer).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_repair).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_repair).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_amount).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_amount).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_tracking).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_tracking).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_notes).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_notes).isNull(), 1);


    cut->set_state(SPartRequest::State::InWork);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Статус изменён на %1").arg(partRequestStatesListModel->getDisplayRole(SPartRequest::State::InWork))), 1);

    // эти методы вызываются перед каждым коммитом модели в tabPartRequest::setModelData(); в этом тесте передаём нули и пустые строки, как буд-то поля остались не заполненными
    cut->set_dealer(0);
    cut->set_client(0);
    cut->set_repair(0);
    cut->set_amount(0);
    cut->set_tracking("");
    cut->set_notes("");
    QCOMPARE(cut->data(SPartRequest::C_client).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_client).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_dealer).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_dealer).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_repair).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_repair).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_amount).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_amount).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_tracking).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_tracking).isNull(), 1);
    QCOMPARE(cut->data(SPartRequest::C_notes).isValid(), 1);
    QCOMPARE(cut->data(SPartRequest::C_notes).isNull(), 1);
    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_priorityChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    cut->initSqlQuery(db);
    cut->load(testData.value("id").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->set_priority(SPartRequest::Priority::High);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Приоритет изменён на %1").arg(partRequestPrioritiesListModel->getDisplayRole(SPartRequest::Priority::High))), 1);

    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_countChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    int qty = testData.value("qty").toInt();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    cut->initSqlQuery(db);
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->count(), testData.value("qty").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->set_count(qty+1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Количество изменёно на %1").arg(qty+1)), 1);

    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_planEndDateChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    cut->initSqlQuery(db);
    cut->load(testData.value("id").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QDate date = QDateTime::currentDateTime().addDays(8).date();
    cut->set_planEndDate(date);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Указана плановая дата завершения: %1").arg(date.toString("dd.MM.yyyy"))), 1);

    cut->commit();

    date = QDateTime::currentDateTime().addDays(15).date();
    cut->set_planEndDate(date);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Плановая дата завершения изменёна на %1").arg(date.toString("dd.MM.yyyy"))), 1);

    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_amountChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    cut->initSqlQuery(db);
    cut->load(testData.value("id").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    double amount = 1200;
    cut->set_amount(amount);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Указана сумма: %1").arg(sysLocale.toCurrencyString(amount))), 1);

    cut->commit();

    amount = 1400;
    cut->set_amount(amount);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Сумма изменёна на %1").arg(sysLocale.toCurrencyString(amount))), 1);

    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_trackChange()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    cut->initSqlQuery(db);
    cut->load(testData.value("id").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QString track = "CN673544564658RU";
    cut->set_tracking(track);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Указан трэк-номер: %1").arg(track)), 1);

    cut->commit();

    track = "CN673844564658RU";
    cut->set_tracking(track);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Трэк-номер изменён на %1").arg(track)), 1);

    cut->commit();

    track = "";
    cut->set_tracking(track);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Трэк-номер удалён")), 1);

    cut->commit();

    query->exec("ROLLBACK;");
}

void TClassTest::test_loadModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    int signalsCounter = 0;
    auto sl = [&](){signalsCounter++;};
    connect(cut.get(), &SPartRequest::modelUpdated, sl);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));

    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->cache.count(), 0);
    QVERIFY(cut->isModelLoaded() == 1);
    QCOMPARE(cut->id(), testData.value("id").toInt());
    QCOMPARE(signalsCounter, 1);
}

void TClassTest::test_managersGetSet()
{
//    QSKIP("");
    auto cut = std::make_unique<SPartRequest>();
    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logRecord->type(), SLogRecordModel::PartRequest);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    cut->initSqlQuery(db);
    cut->load(testData.value("id").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    // Проверка списка ответственных сотрудников (отмеченные галочками в выпадающем списке)
    QCOMPARE(cut->m_managers->rowCount(), 0);
    QList<int> testManagers = managersList(testData.value("managers"));
    QList<int> cutManagers = cut->managers();
//    QCOMPARE(cutManagers.count(), testManagers.count());

    cutManagers = managersList(testData.value("managersNew"));
    cut->setManagers(cutManagers);
    QCOMPARE(cut->m_managers->rowCount(), 4);   // для managersNew=1,5,6,7 будет 4 строки в модели m_managers
    QCOMPARE(cut->m_notifications->rowCount(), 2);  // но в модели m_notifications будет только 2 строки, т. к. для 1 (текущ. пользователь) и 7 (инициатор заявки, он же employee) уведомления не нужны

    // изменений данных, относящихся к таблице `parts_request`, не произошло, и коммит будет пустой (коммит менеджеров нужно выполнять отдельно)
    cut->commit();

    query->exec("ROLLBACK;");
}

QTEST_MAIN(TClassTest)

#include "tclasstest.moc"
