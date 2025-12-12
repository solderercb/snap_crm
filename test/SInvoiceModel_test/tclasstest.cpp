#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "../../models/sinvoicemodel.h"
#include <SLogRecordModel>
#include <ProjectGlobals>

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();
private:
    std::unique_ptr<TDebugStuff> debugStuff;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newEmptyModel();
    void test_query_generators();
    void test_extra_fields();
    void test_commit();
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
}

void TClassTest::cleanupTestCase()
{

}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database(TdConn::main()).isOpen() == 1);
    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
}

void TClassTest::test_newEmptyModel()
{
    auto cut = std::make_unique<SInvoiceModel>();

    QVERIFY(cut->m_record->count() == 14);
    QVERIFY(cut->cache.size() == 0);
    QVERIFY(cut->checkObligatoryFields() == 0);
}

void TClassTest::test_query_generators()
{
    auto cut = std::make_unique<SInvoiceModel>();
    QMap<QString, QVariant> testData;
    debugStuff->testRecordExists("invoice", testData);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->m_record->count(), 16);
    QCOMPARE(cut->id(), testData.value("id").toInt());
    QCOMPARE(cut->state(), SInvoiceModel::State::Payed);
//    qDebug() << "[" << this << "] () | : " << cut->i_query->lastQuery();
    query->exec(QString("SELECT * FROM `information_schema`.`COLUMNS` WHERE `TABLE_SCHEMA` LIKE '%1' AND `TABLE_NAME` LIKE 'invoice';").arg(loginCreds->value("database").toString()));
    QVERIFY(query->size() == cut->m_namesMap->count());
    QVERIFY(cut->isIndexValid(cut->size()) == 0);
    QVERIFY(cut->isIndexValid(SInvoiceModel::C_type) == 1);
    for(int i = 0; i < cut->m_namesMap->count(); i++)
    {
        QVERIFY(query->seek(i) == 1);
        // проверка совпадений имён полей в QSqlRecord, инициализированном при вызове метода SInvoiceModel::load(int),
        // с названиями полей таблицы БД. Названия должны совпадать; см. комментарий к SInvoiceModel::constructSelectQuery()
        QCOMPARE(query->value("COLUMN_NAME").toString(), cut->m_record->fieldName(i));

        cut->setData(i, cut->data(i));  // в cache будут добавлены элементы со статусом None, т. к. данные не изменились
        QCOMPARE(cut->cache.count(), (i+1));
    }
    foreach (auto f, cut->cache)
    {
        f->setData(cut->data(cut->cache.key(f)));
        f->setState(SSingleRowModelBase::ModifiedField::Updated); // данные типа изменились (для проверки генераторов запроса)

    }

//    qDebug() << "[" << this << "] () | : " << cut->constructInsertQuery();
    QCOMPARE(cut->constructInsertQuery().startsWith("INSERT INTO `invoice`(\n  `id`,\n  `num`,\n  `created`,\n  `user`,\n  `seller`,\n  `customer`,\n  `paid`,\n  `tax`,\n  `summ`,\n  `notes`,\n  `total`,\n  `state`,\n  `office`,\n  `type`\n) VALUES ("), 1);
//    qDebug().noquote() << "[" << this << "] () | : " << cut->constructUpdateQuery();
    QRegExp re("UPDATE[^`]+`invoice`\nSET(\n  `[^`]+` = .+){13}\nWHERE `id` = ");
    QCOMPARE(cut->constructUpdateQuery().contains(re), 1);
}

void TClassTest::test_extra_fields()
{
    auto cut = std::make_unique<SInvoiceModel>();
    QMap<QString, QVariant> testData;
    debugStuff->testRecordExists("invoice", testData);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    cut->load(testData.value("id").toInt());
    QCOMPARE(cut->m_record->count(), 16);

    QCOMPARE(cut->covenantorName(), testData.value("covenantorName").toString());
    QCOMPARE(cut->clientId(), testData.value("clientId").toInt());
}

void TClassTest::test_commit()
{
    auto cut = std::make_unique<SInvoiceModel>();
    QMap<QString, QVariant> testData;
    debugStuff->testRecordExists("invoice", testData);

    cut->set_state(SInvoiceModel::Declared);
    cut->set_num(testData.value("num").toString());
    cut->set_seller(testData.value("seller").toInt());
    cut->set_covenantor(testData.value("customer").toInt());
    cut->set_summ(testData.value("summ").toDouble());
    cut->set_total(testData.value("total").toDouble());
    cut->set_notes("test_notes");

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));

    query->exec("BEGIN;");
    QCOMPARE(cut->commit(), 1);
    QVERIFY(cut->id() != 0);
    cut->SSingleRowJModel::load();    // очистка кэша

    cut->set_paid(QDateTime::currentDateTime());
    cut->set_state(SInvoiceModel::Payed);
    QCOMPARE(cut->cache.count(), 2);
    QCOMPARE(cut->commit(), 1);

    query->exec("ROLLBACK;");
}


QTEST_APPLESS_MAIN(TClassTest)

#include "tclasstest.moc"
