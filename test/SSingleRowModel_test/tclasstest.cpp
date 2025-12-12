#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "srepair.h"
#include <SLogRecordModel>

// add necessary includes here

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();
private:
    std::unique_ptr<TDebugStuff> debugStuff;
    QMap<QString, QVariant> testData;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_setTable();
    void test_setPrimaryKey();
    void test_dirty();
    void test_constructSelectQuery();
    void test_constructInsertQuery();
    void test_constructUpdateQuery();
    void test_data_getter();
    void test_obligatoryFields();
    void test_data_setter_new_model();
    void test_data_load();
    void test_data_setter_null_value();
    void test_data_setter_loaded_model();
    void test_last_insert_id();
    void test_log_commit();
    void test_data_commit();
    void test_data_commit_logs();
    void test_report_column_headers();
    void test_report_field();
    void test_report_additional_fields();

};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

void TClassTest::initTestCase()
{
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("SSingleRowModel_test", testData);
}

void TClassTest::cleanupTestCase()
{
}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database(TdConn::main()).isOpen() == 1);
    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
}

void TClassTest::test_newModel()
{
    auto cut = std::make_unique<SRepair>();

    QVERIFY(cut->m_record->count() == 63);
    QVERIFY(cut->cache.size() == 0);
}

void TClassTest::test_setTable()
{
    auto cut = std::make_unique<SRepair>();

    QCOMPARE(cut->table(), "workshop");
    cut->setTable("test-test-test");
    QCOMPARE(cut->table(), "test-test-test");
}

void TClassTest::test_setPrimaryKey()
{
    auto cut = std::make_unique<SRepair>();

    QVERIFY(cut->primaryKeyIndex() == SRepair::C_id);
    cut->setPrimaryKeyIndex(234);
    QVERIFY(cut->primaryKeyIndex() == 234);
}

void TClassTest::test_dirty()
{
    auto cut = std::make_unique<SRepair>();
    QVERIFY(cut->isDirty() == 0);
    QVERIFY(cut->isFieldDirty(SRepair::C_state) == 0);           // ModifiedField::None

    cut->set_state(1);                                           // ModifiedField::Updated
    QVERIFY(cut->isDirty() == 1);
    QVERIFY(cut->isFieldDirty(SRepair::C_state) == 1);

    cut->cache[SRepair::C_state]->m_state = SSingleRowModelBase::ModifiedField::Executed;
    QVERIFY(cut->isDirty() == 0);
    QVERIFY(cut->isFieldDirty(SRepair::C_state) == 0);

    cut->cache[SRepair::C_state]->m_state = SSingleRowModelBase::ModifiedField::Failed;
    QVERIFY(cut->isDirty() == 1);
    QVERIFY(cut->isFieldDirty(SRepair::C_state) == 1);
}

void TClassTest::test_constructSelectQuery()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(123);
    QCOMPARE(cut->constructSelectQuery(), "SELECT\n  `id`,\n  `Hidden`,\n  `Title`,\n  `client`,\n  `type`,\n  `maker`,\n  `model`,\n  `serial_number`,\n  `company`,\n  `office`,\n  `start_office`,\n  `manager`,\n  `current_manager`,\n  `master`,\n  `diagnostic_result`,\n  `in_date`,\n  `out_date`,\n  `state`,\n  `new_state`,\n  `user_lock`,\n  `lock_datetime`,\n  `express_repair`,\n  `quick_repair`,\n  `is_warranty`,\n  `is_repeat`,\n  `payment_system`,\n  `is_card_payment`,\n  `can_format`,\n  `print_check`,\n  `box`,\n  `warranty_label`,\n  `ext_notes`,\n  `is_prepaid`,\n  `prepaid_type`,\n  `prepaid_summ`,\n  `prepaid_order`,\n  `is_pre_agreed`,\n  `is_debt`,\n  `pre_agreed_amount`,\n  `repair_cost`,\n  `real_repair_cost`,\n  `parts_cost`,\n  `fault`,\n  `complect`,\n  `look`,\n  `thirs_party_sc`,\n  `last_save`,\n  `last_status_changed`,\n  `warranty_days`,\n  `barcode`,\n  `reject_reason`,\n  `informed_status`,\n  `image_ids`,\n  `color`,\n  `order_moving`,\n  `early`,\n  `ext_early`,\n  `issued_msg`,\n  `sms_inform`,\n  `invoice`,\n  `cartridge`,\n  `vendor_id`,\n  `termsControl`\nFROM `workshop` WHERE `id` = 123;");
}

void TClassTest::test_constructInsertQuery()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(1);
    cut->set_title("laptop Apple");
    cut->set_clientId(123);
    QCOMPARE(cut->constructInsertQuery(), "INSERT INTO `workshop`(\n  `id`,\n  `Title`,\n  `client`\n) VALUES (\n  1,\n  'laptop Apple',\n  123\n);");
}

void TClassTest::test_constructUpdateQuery()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(123);
    cut->set_title("laptop Apple A1466");
    QCOMPARE(cut->constructUpdateQuery(), "UPDATE\n  `workshop`\nSET\n  `Title` = 'laptop Apple A1466'\nWHERE `id` = 123;");
    cut->cache[SRepair::C_title]->setState(SSingleRowModelBase::ModifiedField::Executed);
    cut->set_clientId(123);
    QCOMPARE(cut->constructUpdateQuery(), "UPDATE\n  `workshop`\nSET\n  `client` = 123\nWHERE `id` = 123;");
}

void TClassTest::test_data_getter()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(123);
    QVERIFY(cut->data(SRepair::C_id) == 123);   // значение из кэша
    QVERIFY(cut->data(SRepair::C_id, SSingleRowModelBase::ModifiedField::Updated) == 123);
    QVERIFY(cut->data(SRepair::C_id, SSingleRowModelBase::ModifiedField::Executed) != 123); //
    cut->cache[0]->setState(SSingleRowModelBase::ModifiedField::Executed);
    QVERIFY(cut->data(SRepair::C_id, SSingleRowModelBase::ModifiedField::Executed) == 123);
    QVERIFY(cut->data(SRepair::C_id) == 123);

    cut->set_title("laptop Apple A1466");
    QCOMPARE(cut->title(), "laptop Apple A1466");
}

void TClassTest::test_obligatoryFields()
{
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->set_title("test_obligatoryFields");
    cut->set_clientId(testData.value("clientId").toInt());
    cut->set_devClass(testData.value("devClass").toInt());
    cut->set_vendor(testData.value("vendor").toInt());
    cut->set_office(testData.value("office").toInt());
    cut->set_manager(testData.value("user").toInt());
    cut->set_diagnosticResult("");
    cut->set_inDate(QDateTime::currentDateTimeUtc());
    cut->set_extNotes(QString());
    cut->set_fault("");
    cut->set_rejectReason("");
    cut->set_company(testData.value("company").toInt());
    cut->set_startOffice(testData.value("office").toInt());
    cut->set_currentManager(testData.value("user").toInt());
    cut->set_engineer(testData.value("engineer").toInt());
    QVERIFY(cut->SSingleRowModel::checkObligatoryFields() == 1);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    query->exec("ROLLBACK;");

    // сразу после коммита модель НЕ должна быть пригодна для ещё одного запроса INSERT
    // (для моделей, которые не обновляются, например, SRepairStatusLog)
    QVERIFY(cut->SSingleRowModel::checkObligatoryFields() == 0);
}

void TClassTest::test_data_setter_new_model()
{
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->setDatabase(db);
    cut->set_title("test_data_setter_new_model");
    cut->set_clientId(testData.value("clientId").toInt());
    cut->set_devClass(testData.value("devClass").toInt());
    cut->set_vendor(testData.value("vendor").toInt());
    cut->set_office(testData.value("office").toInt());
    cut->set_manager(testData.value("user").toInt());
    cut->set_diagnosticResult("");
    cut->set_inDate(QDateTime::currentDateTimeUtc());
    cut->set_extNotes(QString());
    cut->set_fault("");
    cut->set_rejectReason("");
    cut->set_company(testData.value("company").toInt());
    cut->set_startOffice(testData.value("office").toInt());
    cut->set_currentManager(testData.value("user").toInt());
    cut->set_engineer(testData.value("engineer").toInt());

    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model");
    QCOMPARE(cut->commitedData(SRepair::C_title), std::nullopt);

    // при вызове setData с теми же данными в кэше ничего не должно измениться
    cut->set_title("test_data_setter_new_model");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model");
    QCOMPARE(cut->commitedData(SRepair::C_title), std::nullopt);

    // при вызове setData с другими данными в кэше должно измениться только поле m_data соответствующего элемента
    cut->set_title("test_data_setter_new_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model (обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title), std::nullopt);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QVERIFY(cut->commit() == 1);
    // после коммита должен измениться только статус элемента в кэше
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model (обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title), "test_data_setter_new_model (обновлено)");

    // при вызове setData с теми же данными в кэше ничего не должно измениться
    cut->set_title("test_data_setter_new_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model (обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title), "test_data_setter_new_model (обновлено)");

    // при вызове setData с другими данными в кэше должно измениться все поля соответствующего элемента
    cut->set_title("test_data_setter_new_model (ещё раз обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title), "test_data_setter_new_model (обновлено)");

    // при вызове setData с теми же данными в кэше ничего не должно измениться
    cut->set_title("test_data_setter_new_model (ещё раз обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title), "test_data_setter_new_model (обновлено)");

    QVERIFY(cut->commit() == 1);
    // после коммита должен измениться только статус элемента в кэше
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_new_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title), "test_data_setter_new_model (ещё раз обновлено)");

    query->exec("ROLLBACK;");
}

void TClassTest::test_data_load()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(testData.value("repair").toInt());
    QCOMPARE(cut->cache.count(), 1);
    cut->load();
    QCOMPARE(cut->cache.count(), 0);    // при загрузке кэш очищается
    QCOMPARE(cut->m_record->count(), 63);
    QCOMPARE(cut->title(), testData.value("title").toString());
    QCOMPARE(cut->prepaidOrder(), 0);   // по умолчанию в поле записывается 0
}

void TClassTest::test_data_setter_null_value()
{
    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_shared<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->setDatabase(db);
    cut->set_title("test_data_setter_null_value");
    cut->set_clientId(testData.value("clientId").toInt());
    cut->set_devClass(testData.value("devClass").toInt());
    cut->set_vendor(testData.value("vendor").toInt());
    cut->set_office(testData.value("office").toInt());
    cut->set_manager(testData.value("user").toInt());
    cut->set_diagnosticResult("");
    cut->set_inDate(QDateTime::currentDateTimeUtc());
    cut->set_extNotes(QString());
    cut->set_fault("");
    cut->set_rejectReason("");
    cut->set_company(testData.value("company").toInt());
    cut->set_startOffice(testData.value("office").toInt());
    cut->set_currentManager(testData.value("user").toInt());
    cut->set_engineer(testData.value("engineer").toInt());
    QVERIFY(cut->setData(SRepair::C_prepaidOrder, QVariant()) == 1);   // по умолчанию в поле записывается 0
    QCOMPARE(cut->cache[SRepair::C_prepaidOrder]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_prepaidOrder]->m_data, QVariant());
    QCOMPARE(cut->commitedData(SRepair::C_prepaidOrder), std::nullopt);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QVERIFY(cut->commit() == 1);

    cut->load();

// Внимание!
// Возможна путаница при работе с NULL. Поле таблицы, в котором записано NULL, в объекте QSqlRecord будет
// представлено пустым значением соответствующего типа (т. е. методы isValid() и isNull() вернут 1).
// Путаница может возникнуть при визуальном контроле с помощью вывода в консоль. Например, поле INT() со значениями
// NULL и 0 в консоли будут выводиться как QVariant(int, 0), а поле VARCHAR() со значениями NULL и "" как QVariant(QString, "").
// СРАВНИВАТЬ ЭТО ЗНАЧЕНИЕ С QVariant() НЕЛЬЗЯ!
    QCOMPARE(cut->data(SRepair::C_prepaidOrder).isNull(), QVariant().isNull());

    query->exec("ROLLBACK;");
}

void TClassTest::test_data_setter_loaded_model()
{
    int signalCounter = 0;
    int expectedSignalCounter = 0;
    auto lambda = [&](){++signalCounter;};
    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_shared<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->setDatabase(db);
    cut->set_title("test_data_setter_loaded_model");
    cut->set_clientId(testData.value("clientId").toInt());
    cut->set_devClass(testData.value("devClass").toInt());
    cut->set_vendor(testData.value("vendor").toInt());
    cut->set_office(testData.value("office").toInt());
    cut->set_manager(testData.value("user").toInt());
    cut->set_diagnosticResult("");
    cut->set_inDate(QDateTime::currentDateTimeUtc());
    cut->set_extNotes(QString());
    cut->set_fault("");
    cut->set_rejectReason("");
    cut->set_company(testData.value("company").toInt());
    cut->set_startOffice(testData.value("office").toInt());
    cut->set_currentManager(testData.value("user").toInt());
    cut->set_engineer(testData.value("engineer").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();

    cut->load();

    QObject::connect(cut.get(), &SRepair::dataChanged, this, lambda);
    QCOMPARE(cut->title(), "test_data_setter_loaded_model");

    // при вызове setData с теми же данными в кэше должен быть элемент-пустышка
    cut->set_title("test_data_setter_loaded_model");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::None);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, QVariant());
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model");
    QCOMPARE(signalCounter, expectedSignalCounter);

    // при вызове setData с другими данными в кэше должны измениться все поля соответствующего элемента
    cut->set_title("test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->prev()->state(), SSingleRowModelBase::ModifiedField::None);
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model");
    QCOMPARE(signalCounter, ++expectedSignalCounter);

    // при вызове setData с теми же данными в кэше ничего не должно измениться
    cut->set_title("test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->prev()->state(), SSingleRowModelBase::ModifiedField::None);
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model");
    QCOMPARE(signalCounter, expectedSignalCounter);

    QVERIFY(cut->commit() == 1);

    // после коммита должен измениться статус элемента в кэше, методы data() и commitedData() должны возвращать идентичные данные
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->prev().get(), nullptr);
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(signalCounter, expectedSignalCounter);

    // при вызове setData с теми же данными в кэше ничего не должно измениться
    cut->set_title("test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(signalCounter, expectedSignalCounter);

    // при вызове setData с другими данными в кэше должно измениться все поля соответствующего элемента
    cut->set_title("test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(signalCounter, ++expectedSignalCounter);

    // при повторном вызове setData с теми же данными в кэше ничего не должно измениться
    cut->set_title("test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(signalCounter, expectedSignalCounter);

    // при вызове setData с данными, идентичными записанным в БД, элемент в кэше должен замениться на предыдущий
    cut->set_title("test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->prev().get(), nullptr);
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(signalCounter, ++expectedSignalCounter);

    // при вызове setData с другими данными в кэше должно измениться все поля соответствующего элемента
    cut->set_title("test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (обновлено)");
    QCOMPARE(signalCounter, ++expectedSignalCounter);

    QVERIFY(cut->commit() == 1);
    // после коммита должен измениться статус элемента в кэше, метод commitedData() должан возвращать идентичные данные
    QCOMPARE(cut->cache[SRepair::C_title]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SRepair::C_title]->m_data, "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->data(SRepair::C_title),          "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(cut->commitedData(SRepair::C_title),  "test_data_setter_loaded_model (ещё раз обновлено)");
    QCOMPARE(signalCounter, expectedSignalCounter);

    query->exec("ROLLBACK;");
}

void TClassTest::test_last_insert_id()
{
    bool queryNewId = 0;
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->setDatabase(db);
    cut->setQueryIdBeforeInsert(queryNewId);
    cut->set_title("test_last_insert_id");
    cut->set_clientId(testData.value("clientId").toInt());
    cut->set_devClass(testData.value("devClass").toInt());
    cut->set_vendor(testData.value("vendor").toInt());
    cut->set_office(testData.value("office").toInt());
    cut->set_manager(testData.value("user").toInt());
    cut->set_diagnosticResult("");
    cut->set_inDate(QDateTime::currentDateTimeUtc());
    cut->set_extNotes(QString());
    cut->set_fault("");
    cut->set_rejectReason(QString());
    cut->set_rejectReason("");              // для теста закомментировать (предыдущая строка задаёт значение NULL и запрос не выполнится из-за ошибки)
    cut->set_company(testData.value("company").toInt());
    cut->set_startOffice(testData.value("office").toInt());
    cut->set_currentManager(testData.value("user").toInt());
    cut->set_engineer(testData.value("engineer").toInt());
    QCOMPARE(cut->data(cut->i_primaryKeyIndex, SSingleRowModelBase::ModifiedField::Executed).isValid(), 0);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QVERIFY(cut->commit() == 1);
    if(!queryNewId)
    {
        QVERIFY(cut->i_query->lastQuery().startsWith("SELECT LAST_INS") == 1);
    }
    QCOMPARE(cut->cache.value(SRepair::C_id)->state(), SSingleRowModelBase::ModifiedField::Executed);
    QVERIFY(cut->id() > 0);
    cut->set_title("test_last_insert_id (обновлено)");
    cut->set_diagnosticResult("сломался");
    cut->commit();
    QVERIFY(cut->i_query->lastQuery().startsWith("UPDATE") == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_log_commit()
{
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    auto cut = std::make_unique<SLogRecordModel>();

    cut->setDatabase(db);
    cut->set_text("test_log_commit в журнал");

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    query->exec("SELECT * FROM `logs` ORDER BY `id` DESC LIMIT 1;");
    QVERIFY(query->first() == 1);
    QCOMPARE(query->value(SLogRecordModel::C_text), "test_log_commit в журнал");
    query->exec("ROLLBACK;");
}

void TClassTest::test_data_commit()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->set_id(testData.value("repair").toInt());
    cut->load();
    cut->load();    // двойная загрузка, чтобы быть уверенным, что заданный primaryKey не стёрся
    QCOMPARE(cut->m_record->count(), 63);

    cut->set_title(cut->title() + "_");

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
    QCOMPARE(cut->title(), QString("%1_").arg(testData.value("title").toString()));
    QVERIFY(cut->cache[SRepair::C_title]->state() == SSingleRowModelBase::ModifiedField::Executed);
    cut->load();
    QCOMPARE(cut->title(), testData.value("title").toString());
}

void TClassTest::test_data_commit_logs()
{
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<SRepair>();

    cut->set_id(testData.value("repair").toInt());
    cut->load();
    QVERIFY(cut->diagnosticResult() != "Ушиб всей бабки"); // нужно, чтобы результат диагностики отличался
    cut->set_diagnosticResult("Ушиб всей бабки");
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.first(), "Результат диагностики изменён на \"Ушиб всей бабки\"");

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QVERIFY(cut->commit() == 1);
    query->exec(QString("SELECT * FROM `logs` WHERE `repair` = %1 ORDER BY `id` DESC LIMIT 1;").arg(testData.value("repair").toInt()));
    QVERIFY(query->first() == 1);
    QCOMPARE(query->value(SLogRecordModel::C_text), "Результат диагностики изменён на \"Ушиб всей бабки\"");
    query->exec("ROLLBACK;");

}

void TClassTest::test_report_column_headers()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(testData.value("repair").toInt());
    cut->load();
    QCOMPARE(cut->reportColumnCount(), 55);
    QVERIFY(cut->m_reportFieldsMap->contains("id") == 1);
    QVERIFY(cut->m_reportFieldsMap->contains("title") == 1);
}

void TClassTest::test_report_field()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(testData.value("repair").toInt());
    cut->load();
    QVariant data;
    cut->reportData("id", data);
    QCOMPARE(data, QVariant(testData.value("repair").toInt()));
    cut->reportData("title", data);
    QCOMPARE(data, testData.value("title").toString());

}

void TClassTest::test_report_additional_fields()
{
    auto cut = std::make_unique<SRepair>();

    cut->set_id(testData.value("repair").toInt());
    cut->load();
    QVariant data;
    cut->reportData("inDateTime", data);
    QCOMPARE(data.toString(), "additional report field inDateTime");
    cut->reportData("outDateTime", data);
    QCOMPARE(data, QVariant("additional report field outDateTime"));
}

QTEST_APPLESS_MAIN(TClassTest)

#include "tclasstest.moc"
