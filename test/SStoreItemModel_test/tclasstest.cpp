#include "windows.h"
#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "../../models/sstoreitemmodel.h"
#include <ProjectGlobals>
#include <SLogRecordModel>

class SStoreItemModelTest : public SStoreItemModel
{
    Q_OBJECT
    friend class TClassTest;
signals:
    void loadQtysSubstitute(itemQtys *&container);
    void commitSubstitute(std::optional<bool> &ret);
public:
    explicit SStoreItemModelTest(QObject *parent = nullptr) :
        SStoreItemModel(parent){};
    virtual itemQtys *loadQtys() override {
        itemQtys *container = nullptr;
        emit loadQtysSubstitute(container);
        if(container == nullptr)
            container = SStoreItemModel::loadQtys();
        return container;
    };
    bool commit() override {
        std::optional<bool> ret = std::nullopt;
        emit commitSubstitute(ret);
        if(ret)
            return ret.value();

        return SStoreItemModel::commit();
    };
};

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
    std::shared_ptr<SStoreItemModelTest> prepareModel();
    void cleanupTestCase();
    void test_connection();
    void test_newEmptyModel();
    void test_newModelFromRec();
    void test_sale();
    void test_unsale();
    void test_reserve();
    void test_saleStore();
    void test_saleRepair();
    void test_unsaleRepair();
    void test_free();
    void test_writeOff();
    void test_cancelWriteOff();
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
    debugStuff->readTestData("SStoreItemModel_test", testData);
}

std::shared_ptr<SStoreItemModelTest> TClassTest::prepareModel()
{
    auto cut = std::make_shared<SStoreItemModelTest>();

//    cut->setPrimaryKey(3600);
    cut->initFieldWithPrevLoaded(SStoreItemModel::C_id, testData.value("id").toInt());
    cut->setNewExpense(1); // Кол-во из таблицы в программе

    // Стоимость из таблицы в программе; может быть произвольной; она используется при рассчете роялти поставщика если товар на реализации и должна передаваться из модели SSaleTableModel
    cut->initFieldWithPrevLoaded(SStoreItemModel::C_price, testData.value("price").toDouble());

    // TODO: слудующие поля по идее можно загружать непосредственно во время коммита, а не передавать из модели SSaleTableModel (и соответственно не загружать их при добавлении в таблицу)
    cut->initFieldWithPrevLoaded(SStoreItemModel::C_isRealization, 0);
    cut->initFieldWithPrevLoaded(SStoreItemModel::C_returnPercent, 0);
    cut->initFieldWithPrevLoaded(SStoreItemModel::C_inPrice, testData.value("inPrice").toDouble());
    cut->initFieldWithPrevLoaded(SStoreItemModel::C_dealer, testData.value("client").toInt());

    return cut;
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
    auto cut = std::make_unique<SStoreItemModelTest>();

    QVERIFY(cut->m_record->count() == 52);
    QVERIFY(cut->cache.size() == 0);
}

void TClassTest::test_newModelFromRec()
{
    auto cut = prepareModel();

    QCOMPARE(cut->id(), testData.value("id").toInt());
    QCOMPARE(cut->price(), testData.value("price").toDouble());
    QCOMPARE(cut->isRealization(), 0);
    QCOMPARE(cut->returnPercent(), 0);
    QCOMPARE(cut->inPrice(), testData.value("inPrice").toDouble());
    QCOMPARE(cut->dealer(), testData.value("client").toInt());
    QCOMPARE(cut->m_newExpense, 1);
    QCOMPARE(cut->m_saleObjId, 0);
}

void TClassTest::test_sale()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 0;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count--;
        item->sold = sold++;
        item->reserved = reserved;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::Sale);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_count]->data().toInt(), 122);
        QCOMPARE(cut->cache[SStoreItemModel::C_sold]->data().toInt(), 78);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->setDatabase(db);

    cut->setSaleMode(SStoreItemModelTest::SaleMode::Store);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QCOMPARE(cut->sale("Тест продажи"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 1);
}

void TClassTest::test_unsale()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 0;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count++;
        item->sold = sold--;
        item->reserved = reserved;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::Unsale);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_count]->data().toInt(), 124);
        QCOMPARE(cut->cache[SStoreItemModel::C_sold]->data().toInt(), 76);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->m_commitedExpense = 1;
    cut->setDatabase(db);
    cut->setSaleMode(SStoreItemModelTest::SaleMode::Store);

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->unsale("Тест возврата"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 0);
}

void TClassTest::test_reserve()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 0;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count;
        item->sold = sold;
        item->reserved = reserved++;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::Reserve);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_reserved]->data().toInt(), 1);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->setDatabase(db);

    cut->setSaleMode(SStoreItemModelTest::SaleMode::Store);
    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->reserve("Тест резерва"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 1);
}

void TClassTest::test_saleStore()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 1;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count--;
        item->sold = sold++;
        item->reserved = reserved--;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::SaleReserved);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_count]->data().toInt(), 122);
        QCOMPARE(cut->cache[SStoreItemModel::C_sold]->data().toInt(), 78);
        QCOMPARE(cut->cache[SStoreItemModel::C_reserved]->data().toInt(), 0);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->setDatabase(db);
    cut->setSaleMode(SStoreItemModelTest::SaleMode::Store);
    cut->m_commitedExpense = 1;

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->saleStore("Тест продажа зарезервированного товара"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 1);
}

void TClassTest::test_saleRepair()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 1;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count--;
        item->sold = sold++;
        item->reserved = reserved--;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::SaleReserved);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_count]->data().toInt(), 122);
        QCOMPARE(cut->cache[SStoreItemModel::C_sold]->data().toInt(), 78);
        QCOMPARE(cut->cache[SStoreItemModel::C_reserved]->data().toInt(), 0);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->setDatabase(db);
    cut->setSaleMode(SStoreItemModelTest::SaleMode::Repair);
    cut->m_commitedExpense = 1;

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->saleRepair("Тест списания товара при выдаче ремонта"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 1);
}

void TClassTest::test_unsaleRepair()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 0;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count++;
        item->sold = sold--;
        item->reserved = reserved++;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::UnsaleRepair);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_count]->data().toInt(), 124);
        QCOMPARE(cut->cache[SStoreItemModel::C_sold]->data().toInt(), 76);
        QCOMPARE(cut->cache[SStoreItemModel::C_reserved]->data().toInt(), 1);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->setDatabase(db);
    cut->setSaleMode(SStoreItemModelTest::SaleMode::Repair);
    cut->m_commitedExpense = 1;

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->unsaleRepair("Тест возврата товара при отмене выдачи ремонта"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 1);
}

void TClassTest::test_free()
{
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);
    int count = 123, sold = 77, reserved = 5;
    auto loadQtys = [&](SStoreItemModel::itemQtys*& item){
        item = new SStoreItemModel::itemQtys;
        item->count = count;
        item->sold = sold;
        item->reserved = reserved--;
    };
    connect(cut.get(), &SStoreItemModelTest::loadQtysSubstitute, this, loadQtys);

    auto commit = [&](std::optional<bool> &ret){
        QCOMPARE(cut->m_opOnItemType, SStoreItemModel::OpOnItem::Free);
        QCOMPARE(cut->cache.count(), 3);
        QCOMPARE(cut->cache[SStoreItemModel::C_reserved]->data().toInt(), 4);
        QCOMPARE(cut->i_logTexts.count(), 1);
        ret = cut->SStoreItemModel::commit();
    };
    connect(cut.get(), &SStoreItemModelTest::commitSubstitute, this, commit);

    cut->setDatabase(db);
    cut->setSaleMode(SStoreItemModelTest::SaleMode::Repair);
    cut->m_commitedExpense = 1;

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->free("Тест возврата товара на склад (удаления из корзины)"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 0);
}

void TClassTest::test_writeOff()
{
    QSKIP("Не реализован метод SStoreItemModelTest::writeOff()");
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);

    cut->setDatabase(db);

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->writeOff("Тест списания товара"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 1);
}

void TClassTest::test_cancelWriteOff()
{
    QSKIP("Не реализован метод SStoreItemModelTest::cancelWriteOff()");
    auto cut = prepareModel();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_shared<QSqlQuery>(db);

    cut->setDatabase(db);

    query->exec(QString("# =========================================== %1").arg(__func__));
    debugStuff->startSqlLog(db, __func__);
    query->exec("BEGIN;");
    QCOMPARE(cut->cancelWriteOff("Тест возврата товара при отмене списания"), 1);

    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_commitedExpense, 0);
}

//QTEST_APPLESS_MAIN(TClassTest)
int main(int argc, char *argv[])
{
    TESTLIB_SELFCOVERAGE_START(TClassTest)
    QT_PREPEND_NAMESPACE(QTest::Internal::callInitMain)<TClassTest>();
    QApplication app(argc, argv);
    SetConsoleOutputCP(65001);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    TClassTest tc;
    QTest::setMainSourcePath(__FILE__);
    return QTest::qExec(&tc, argc, argv);
}

#include "tclasstest.moc"
