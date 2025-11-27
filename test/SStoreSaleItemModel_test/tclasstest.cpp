#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "global.h"
#include "../../models/sstoresaleitemmodel.h"
#include "ssaletablemodel.h"
#include <SStoreItemModel>
#include <SLogRecordModel>

// специальная структура для инициализации полей SStoreSaleItemModel
struct InitStruct {
public:
    InitStruct(QVariant init_data, bool init_changedFlag = 0, std::optional<QVariant> init_oldData = std::nullopt) :
        data(init_data),
        changedFlag(init_changedFlag),
        oldData(init_oldData){
    };
    QVariant data;
    bool changedFlag;
    std::optional<QVariant> oldData;
};

class TClassTest : public QObject
{
    Q_OBJECT
signals:
    void itemInitSubstitute(int index, InitStruct *item);
public:
    TClassTest();
    ~TClassTest();
    std::shared_ptr<SStoreSaleItemModel> prepareModel(const int primaryKey);
private:
    std::unique_ptr<TDebugStuff> debugStuff;
    QMap<QString, QVariant> testData;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newEmptyModel();
    void test_sale();
    void test_reserve();
    void test_saleReserved();
    void test_saleReservedIncrCount();
    void test_saleReservedDecrCount();
    void test_unsale();
    void test_free();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

std::shared_ptr<SStoreSaleItemModel> TClassTest::prepareModel(const int primaryKey)
{
    auto cut = std::make_shared<SStoreSaleItemModel>();
    auto initializer = [&](const int index, InitStruct *strct){
        if(strct->changedFlag)
        {
            if(strct->oldData)  // сначала запись старого значения
                cut->initFieldWithPrevLoaded(index, strct->oldData.value_or(QVariant()));

            cut->setData(index, strct->data);
        }
        else
            cut->initFieldWithPrevLoaded(index, strct->data);
    };

    for(int i = 0; i < cut->size(); i++)
    {
        InitStruct *strct;

        // инициализация структуры значением по умолчанию
        switch (i)
        {
            case SStoreSaleItemModel::C_id: strct = new InitStruct(primaryKey, primaryKey?0:1); break; // id записи в таблице store_sales
            case SStoreSaleItemModel::C_count: strct = new InitStruct(testData.value("count").toInt()); break; // Кол-во из таблицы в программе
            case SStoreSaleItemModel::C_price: strct = new InitStruct(testData.value("price").toDouble()); break; // Стоимость из таблицы в программе
            case SStoreSaleItemModel::C_sn: strct = new InitStruct(""); break; // sn из таблицы в программе
            case SStoreSaleItemModel::C_warranty: strct = new InitStruct(testData.value("warranty").toInt()); break; // гарантия из таблицы в программе
            case SStoreSaleItemModel::C_user: strct = new InitStruct(userDbData->id()); break; // код пользователя
            case SStoreSaleItemModel::C_isRealization: strct = new InitStruct(0); break;
            case SStoreSaleItemModel::C_returnPercent: strct = new InitStruct(0); break;
            case SStoreSaleItemModel::C_state: strct = new InitStruct(0); break; // состояние; break; например, 1 - в корзине у сотрудника, 4 - архив
            case SStoreSaleItemModel::C_itemId: strct = new InitStruct(testData.value("item").toInt()); break;
            case SStoreSaleItemModel::C_inPrice: strct = new InitStruct(testData.value("inPrice").toDouble()); break;
            case SStoreSaleItemModel::C_documentId: strct = new InitStruct(QVariant(QVariant::Int)); break; // id РН
            case SStoreSaleItemModel::C_dealer: strct = new InitStruct(testData.value("dealer").toInt()); break; // `store_items`.`dealer`
            case SStoreSaleItemModel::C_buyer: strct = new InitStruct(testData.value("client").toInt()); break; // id клиента (используется при обычной продаже)
            case SStoreSaleItemModel::C_realizatorPayed: strct = new InitStruct(0); break; // id клиента (используется при обычной продаже)
            default: strct = new InitStruct(QVariant()); break;
        }

        // по сигналу изменение данных для каждого конкретного теста
        emit itemInitSubstitute(i, strct);

        // запись данных в поле тестируемого класса
        initializer(i, strct);

        delete strct;
    }

    return cut;
}

void TClassTest::initTestCase()
{
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("SStoreSaleItemModel_test", testData);
    allUsersMap->insert(1, "test");
}

void TClassTest::cleanupTestCase()
{

}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database("connMain").isOpen() == 1);
    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
}

void TClassTest::test_newEmptyModel()
{
//    QSKIP("");
    auto cut = std::make_unique<SStoreSaleItemModel>();

//    QVERIFY(cut->m_record->count() == 15);
//    QVERIFY(cut->cache.size() == 0);
//    QVERIFY(cut->m_storeItem == nullptr);
}

void TClassTest::test_sale()
{
//    QSKIP("");
    int id = 0;

    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_count: strct->data = 2; strct->changedFlag = true; break;
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); strct->changedFlag = true; break;
            default: strct->changedFlag = true;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);

    QCOMPARE(cut->id(), id);
    QCOMPARE(cut->itemId(), testData.value("item").toInt());
    QCOMPARE(cut->price(), testData.value("price").toDouble());
    QCOMPARE(cut->commitedData(SStoreSaleItemModel::C_count)->toInt(), 0);
    QCOMPARE(cut->state(), SStoreSaleItemModel::Active);
    QCOMPARE(cut->documentId(), testData.value("document").toInt());
    QCOMPARE(cut->i_logRecord->document(), testData.value("document").toInt());
    QCOMPARE(cut->sn(), "");
    QCOMPARE(cut->warranty(), testData.value("warranty").toInt());

    QCOMPARE(cut->cache.count(), 18);

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    QCOMPARE(cut->sale(), 1);
    QCOMPARE(cut->m_storeItem->m_logText, tr("Продажа товара в кол-ве %1ед.").arg(cut->count()));
    query->exec("ROLLBACK;");

    QVERIFY(cut->id() != 0);
    QCOMPARE(cut->m_storeItem->m_lastOp, 5);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

void TClassTest::test_reserve()
{
//    QSKIP("");
    int id = 0;

    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); strct->changedFlag = true; break;
            default: strct->changedFlag = true;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);

    QCOMPARE(cut->commitedData(SStoreSaleItemModel::C_count)->toInt(), 0);
    // проверка других полей класса в методе test_sale()

    QCOMPARE(cut->cache.count(), 18);

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->reserve();
    QCOMPARE(cut->m_storeItem->m_logText, tr("Резерв %1ед. товара").arg(cut->count()));
    query->exec("ROLLBACK;");

    QVERIFY(cut->id() != 0);
    QCOMPARE(cut->m_storeItem->m_lastOp, 1);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

void TClassTest::test_saleReserved()
{
//    QSKIP("");
    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_count: strct->data = testData.value("count").toInt(); break;
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(testData.value("id").toInt());
    disconnect(this);

    QCOMPARE(cut->count(), testData.value("count").toInt());
    // проверка других полей класса в методе test_sale()

    QCOMPARE(cut->cache.count(), 0);    // при продаже ранее зарезервированного товара в таблице store_sales ничего не зименяется

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->sale();
    QCOMPARE(cut->m_storeItem->m_logText, tr("Продажа ранее зарезервированного товара: %1ед.").arg(cut->count()));
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_storeItem->m_lastOp, 6);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

void TClassTest::test_saleReservedIncrCount()
{
//    QSKIP("");
    int newValue = 3;
    int oldValue = testData.value("count").toInt();

    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_count: strct->data = newValue; strct->changedFlag = true; strct->oldData = oldValue; break;
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(testData.value("id").toInt());
    disconnect(this);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->commitedData(SStoreSaleItemModel::C_count)->toInt(), oldValue);

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->sale();
    QCOMPARE(cut->m_storeItem->m_logText, tr("Продажа ранее зарезервированного товара: %1ед.; дополнительно: %2ед.").arg(oldValue).arg(cut->count() - oldValue));
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_storeItem->m_lastOp, 6);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

void TClassTest::test_saleReservedDecrCount()
{
//    QSKIP("");
    int newValue = testData.value("count").toInt();
    int oldValue = newValue+2;

    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_count: strct->data = newValue; strct->changedFlag = true; strct->oldData = oldValue; break;
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(testData.value("id").toInt());
    disconnect(this);

    QCOMPARE(cut->commitedData(SStoreSaleItemModel::C_count)->toInt(), oldValue);
    // проверка других полей класса в методе test_sale()

    QCOMPARE(cut->cache.count(), 1);

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->sale();
    QCOMPARE(cut->m_storeItem->m_logText, tr("Продажа ранее зарезервированного товара: %1ед.; отмена резерва (невостребовано): %2ед.").arg(cut->count()).arg(oldValue - cut->count()));
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_storeItem->m_lastOp, 6);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

void TClassTest::test_unsale()
{
//    QSKIP("");
    int count = testData.value("count").toInt();

    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_count: strct->data = 0; strct->changedFlag = true; strct->oldData = count; break;
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(testData.value("id").toInt());
    disconnect(this);

    // проверка других полей класса в методе test_sale()
    cut->set_unsaleReason("test_unsale");

    QCOMPARE(cut->cache.count(), 2);

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->unsale();
    QCOMPARE(cut->m_storeItem->m_logText, tr("Возврат %1ед. товара, причина: %2").arg(count).arg(cut->unsaleReason()));
    QCOMPARE(cut->state(), SStoreSaleItemModel::Cancelled);
    QCOMPARE(cut->cache.count(), 3);
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_storeItem->m_lastOp, 7);
    QCOMPARE(cut->m_storeItem->m_commited, 1);

}

void TClassTest::test_free()
{
//    QSKIP("");
    int count = testData.value("count").toInt();

    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SStoreSaleItemModel::C_count: strct->data = 0; strct->changedFlag = true; strct->oldData = count; break;
            case SStoreSaleItemModel::C_documentId: strct->data = testData.value("document").toInt(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(testData.value("id").toInt());
    disconnect(this);

    // проверка других полей класса в методе test_sale()

    QCOMPARE(cut->cache.count(), 1);

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->free();
    QCOMPARE(cut->m_storeItem->m_logText, tr("Отмена резерва %1ед. товара").arg(count));
    QCOMPARE(cut->state(), SStoreSaleItemModel::Cancelled);
    QCOMPARE(cut->unsaleReason(), "cancel reserve");
    QCOMPARE(cut->cache.count(), 3);
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_storeItem->m_lastOp, 4);
    QCOMPARE(cut->m_storeItem->m_commited, 1);

}

QTEST_APPLESS_MAIN(TClassTest)

#include "tclasstest.moc"
