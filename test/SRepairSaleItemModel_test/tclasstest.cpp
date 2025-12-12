#include "windows.h"
#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "global.h"
#include "../../models/srepairsaleitemmodel.h"
#include "ssaletablemodel.h"
#include <SStoreItemModel>
#include <SLogRecordModel>

/* специальная структура для инициализации полей SStoreSaleItemModel
 * data значение поля
 * changedFlag признак обновлённого значения: если 0, то init_data будет записано в SSingleRowModelBase::m_record, если 1 — в кэш
 * oldData "предыдущее" значение, будет записано в SSingleRowModelBase::m_record
 * skip указание на пропуск поля (т. е. поле останется не инициализированным)
*/
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
    bool skip = 0;
};

class TClassTest : public QObject
{
    Q_OBJECT
signals:
    void itemInitSubstitute(int index, InitStruct *item);
public:
    TClassTest();
    ~TClassTest();
    std::shared_ptr<SRepairSaleItemModel> prepareModel(const int primaryKey);
private:
    std::unique_ptr<TDebugStuff> debugStuff;
    QMap<QString, QVariant> testData;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newEmptyModel();
    void test_addItemFromBasket();
    void test_addItemFromBasket2();
    void test_addItemFromWarehouse();
    void test_loadBoqAndEdit();
    void test_loadBoqAndRemoveItem();
    void test_loadBoqAndRemoveItem2();
    void test_loadBoqAndIssue();
    void test_addItemToBasket();
    void test_addItemToBasket2();
    void test_requestItem();
    void test_returnItemToWarehouse();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

std::shared_ptr<SRepairSaleItemModel> TClassTest::prepareModel(const int primaryKey)
{
    // такой же код инициализации используется в тесте SStoreItemModel
    auto cut = std::make_shared<SRepairSaleItemModel>();
    auto initializer = [&](const int index, InitStruct *strct){
        if(strct->skip)
            return;

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
        switch(i)
        {
            case SRepairSaleItemModel::C_id: strct = new InitStruct(primaryKey, primaryKey?0:1); break; // id записи в таблице store_int_reserve
            case SRepairSaleItemModel::C_itemId: strct = new InitStruct(testData.value("itemId").toInt()); break;
            case SRepairSaleItemModel::C_name: strct = new InitStruct(testData.value("itemName").toString()); break;
            case SRepairSaleItemModel::C_count: strct = new InitStruct(1); break; // Кол-во из таблицы в программе
            case SRepairSaleItemModel::C_created: strct = new InitStruct(QVariant()); break;
            case SRepairSaleItemModel::C_toUser: strct = new InitStruct(0); break; // код пользователя
            case SRepairSaleItemModel::C_notes: strct = new InitStruct(QVariant()); break;
            case SRepairSaleItemModel::C_state: strct = new InitStruct(0); break; // состояние; например, 1 - в корзине у сотрудника, 4 - архив
            case SRepairSaleItemModel::C_repairId: strct = new InitStruct(0); break; // id ремонта
            case SRepairSaleItemModel::C_workId: strct = new InitStruct(QVariant()); break; // используется для правильного порядка в таблице в карточке ремонта
            case SRepairSaleItemModel::C_price: strct = new InitStruct(testData.value("price").toDouble()); break; // Стоимость из таблицы в программе
            case SRepairSaleItemModel::C_sn: strct = new InitStruct(""); break; // sn из таблицы в программе
            case SRepairSaleItemModel::C_warranty: strct = new InitStruct(testData.value("warranty").toInt()); break; // гарантия из таблицы в программе
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
    debugStuff->readTestData("SRepairSaleItemModel_test", testData);
    allUsersMap->insert(1, "test");
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
//    QSKIP("");
    auto cut = std::make_unique<SRepairSaleItemModel>();

    QVERIFY(cut->m_record->count() == 15);
    QVERIFY(cut->cache.size() == 0);
    QVERIFY(cut->m_storeItem.get() != nullptr);
}

/* Добавление детали из своей корзины с одновременным изменением кол-ва */
void TClassTest::test_addItemFromBasket()
{
//    QSKIP("");
    int id = testData.value("id").toInt();
    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SRepairSaleItemModel::C_state: strct->data = SRepairSaleItemModel::EngineerBasket; break;
            case SRepairSaleItemModel::C_repairId: strct->data = 0; break;
            case SRepairSaleItemModel::C_workId: strct->data = 0; break;
            case SRepairSaleItemModel::C_notes: strct->data = testData.value("notes").toString(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);

    // изменение статуса и установка номера ремонта должны происходить только после инициализации модели
    cut->set_repairId(testData.value("repair").toInt());
    cut->set_state(SRepairSaleItemModel::RepairLinked);

    cut->set_count(2);  // изменение кол-ва после изменения статуса

    QCOMPARE(cut->id(), id);
    QCOMPARE(cut->itemId(), testData.value("itemId").toInt());
    QCOMPARE(cut->name(), testData.value("itemName").toString());
    QCOMPARE(cut->price(), testData.value("price").toDouble());
    QCOMPARE(cut->count(), testData.value("count").toInt());
    QCOMPARE(cut->notes(), testData.value("notes").toString()); // при доабвлении товара из корзины запись не изменяется
    QCOMPARE(cut->state(), SRepairSaleItemModel::RepairLinked);
    QCOMPARE(cut->repairId(), testData.value("repair").toInt());
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    QCOMPARE(cut->workId(), 0);
    QCOMPARE(cut->sn(), "");
    QCOMPARE(cut->warranty(), testData.value("warranty").toInt());

    QCOMPARE(cut->cache.count(), 3);

    QString testStr;
    QCOMPARE(cut->i_logTexts.count(), 2);
    testStr = tr("Добавлен товар \"%1\" стоимостью %2 в кол-ве %3ед")
              .arg(cut->name())
              .arg(sysLocale.toCurrencyString(cut->price()))
              .arg(1);
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    testStr = tr("[А] Количество товара \"%1\" изменёно с %2 на %3")
              .arg(cut->name())
              .arg(1)
              .arg(2);
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->m_storeItem->m_commitedExpense, 1);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_storeItem->m_lastOp, 1);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

/* Добавление детали из корзины другого сотрудника */
void TClassTest::test_addItemFromBasket2()
{
//    QSKIP("");
    int id = testData.value("id").toInt();
    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SRepairSaleItemModel::C_state: strct->data = SRepairSaleItemModel::EngineerBasket; break;
            case SRepairSaleItemModel::C_toUser: strct->data = userDbData->id(); break;
            case SRepairSaleItemModel::C_repairId: strct->data = 0; break;
            case SRepairSaleItemModel::C_workId: strct->data = 0; break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);

    // изменение статуса и установка номера ремонта должны происходить только после инициализации модели
    cut->set_toUser(2);
    cut->set_repairId(testData.value("repair").toInt());
    cut->set_state(SRepairSaleItemModel::RepairLinked);

    QCOMPARE(cut->id(), id);
    QCOMPARE(cut->itemId(), testData.value("itemId").toInt());
    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->name(), testData.value("itemName").toString());
    QCOMPARE(cut->price(), testData.value("price").toDouble());
    QCOMPARE(cut->count(), 1);
    QCOMPARE(cut->state(), SRepairSaleItemModel::RepairLinked);
    QCOMPARE(cut->repairId(), testData.value("repair").toInt());
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    QCOMPARE(cut->workId(), 0);
    QCOMPARE(cut->sn(), "");
    QCOMPARE(cut->warranty(), testData.value("warranty").toInt());

    QCOMPARE(cut->cache.count(), 3);

    QString testStr;
    QCOMPARE(cut->i_logTexts.count(), 1);
    testStr = tr("Добавлен товар \"%1\" стоимостью %2 в кол-ве %3ед")
              .arg(cut->name())
              .arg(sysLocale.toCurrencyString(cut->price()))
              .arg(cut->count());
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    QCOMPARE(cut->m_storeItem->i_logTexts.count(), 1);
    testStr = tr("[A] Товар \"%1\" перемещён из корзины сотрудника %2 в корзину сотрудника %3")
              .arg(cut->name(),
              usersModel->getDisplayRole(cut->commitedData(SRepairSaleItemModel::C_toUser).value().toInt()),
              usersModel->getDisplayRole(cut->toUser()));

    QCOMPARE(cut->m_storeItem->i_logTexts.values().contains(testStr), 1);
    QCOMPARE(cut->m_storeItem->m_commitedExpense, 1);
    QCOMPARE(cut->m_storeItem->m_lastOp, -1);
    QCOMPARE(cut->m_storeItem->m_commited, 0);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->m_storeItem->m_lastOp, 5);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

/* Добавление детали со склада
 * Этот сценарий возможен если пользователь обладает правом "Брать товары со склада" */
void TClassTest::test_addItemFromWarehouse()
{
//    QSKIP("");
    int id = 0;
    auto itemSubstitute = [&](int index, InitStruct *strct){
        strct->changedFlag = true;
        switch(index)
        {
            case SRepairSaleItemModel::C_id: strct->skip = 1; break;
            case SRepairSaleItemModel::C_created: strct->skip = 1; break;
            case SRepairSaleItemModel::C_fromUser: strct->skip = 1; break;
            case SRepairSaleItemModel::C_rLock: strct->skip = 1; break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);

    // изменение статуса и установка номера ремонта должны происходить только после инициализации модели
    cut->set_toUser(userDbData->id());
    cut->set_repairId(testData.value("repair").toInt());
    cut->set_state(SRepairSaleItemModel::RepairLinked);
    cut->set_sn(testData.value("newSn").toString());
    cut->set_warranty(testData.value("newWarranty").toInt());

    QString testStr;
    QCOMPARE(cut->isPrimaryKeyValid(), false);
    QCOMPARE(cut->isModelLoaded(), false);
    QCOMPARE(cut->itemId(), testData.value("itemId").toInt());
    QCOMPARE(cut->name(), testData.value("itemName").toString());
    QCOMPARE(cut->price(), testData.value("price").toDouble());
    QCOMPARE(cut->count(), 1);
    testStr = tr("Автовыдача %1ед. товара сотруднику %2 для ремонта №%3")
              .arg(cut->count())
              .arg(allUsersModel->getDisplayRole(cut->toUser()))
              .arg(cut->repairId());
    QCOMPARE(cut->notes(), testStr);
    QCOMPARE(cut->state(), SRepairSaleItemModel::RepairLinked);
    QCOMPARE(cut->repairId(), testData.value("repair").toInt());
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    QCOMPARE(cut->data(SRepairSaleItemModel::C_workId), QVariant());    // таблица ещё не сохранена, id работы неизвестен
    QCOMPARE(cut->sn(), testData.value("newSn").toString());
    QCOMPARE(cut->warranty(), 186);

    QCOMPARE(cut->cache.count(), 11);

    QCOMPARE(cut->i_logTexts.count(), 3);
    testStr = tr("Добавлен товар \"%1\" стоимостью %2 в кол-ве %3ед")
              .arg(cut->name())
              .arg(sysLocale.toCurrencyString(cut->price()))
              .arg(cut->count());
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    testStr = tr("Серийный номер товара \"%1\" установлен \"%2\"")
              .arg(cut->name())
              .arg(testData.value("newSn").toString());
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    testStr = tr("Срок гарантии на товар \"%1\" установлен \"%2\"")
              .arg(cut->name())
              .arg(warrantyTermsModel->getDisplayRole(186, 1));
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->m_storeItem->m_commitedExpense, 0);
    QCOMPARE(cut->m_storeItem->m_lastOp, -1);
    QCOMPARE(cut->m_storeItem->m_commited, 0);
}

/* Загрузка ранее сохранённых данных и их правка
 * Сценарий: пользователь открыл карточку ремонта, что-то изменил в списке работ и деталей и сохранил
*/
void TClassTest::test_loadBoqAndEdit()
{
    int id = testData.value("id").toInt();
    int countNew = 2, countOld = 3;
    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SRepairSaleItemModel::C_notes: strct->data = "Автовыдача 1ед. товара сотруднику test для ремонта №X"; break;
            case SRepairSaleItemModel::C_repairId: strct->data = testData.value("repair").toInt(); break;
            case SRepairSaleItemModel::C_workId: strct->data = 1234; break;
            case SRepairSaleItemModel::C_state: strct->data = SRepairSaleItemModel::RepairLinked; break;
            case SRepairSaleItemModel::C_count: strct->data = countOld; break;
            case SRepairSaleItemModel::C_price: strct->data = testData.value("price").toDouble(); break;
            case SRepairSaleItemModel::C_sn: strct->data = ""; break;
            case SRepairSaleItemModel::C_warranty: strct->data = testData.value("warranty").toInt(); break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);
    QCOMPARE(cut->cache.count(), 0);
    cut->set_count(countNew);
    cut->set_price(testData.value("newPrice").toDouble());
    cut->set_sn(testData.value("newSn").toString());
    cut->set_warranty(testData.value("newWarranty").toInt());

    QCOMPARE(cut->id(), id);
    QCOMPARE(cut->itemId(), testData.value("itemId").toInt());
    QCOMPARE(cut->name(), testData.value("itemName").toString());
    QCOMPARE(cut->price(), testData.value("newPrice").toDouble());
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_price)->toInt(), testData.value("price").toDouble());
    QCOMPARE(cut->count(), countNew);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_count)->toInt(), countOld);
    QCOMPARE(cut->notes(), "Автовыдача 1ед. товара сотруднику test для ремонта №X"); // при доабвлении товара из корзины запись не изменяется
    QCOMPARE(cut->state(), SRepairSaleItemModel::RepairLinked);
    QCOMPARE(cut->repairId(), testData.value("repair").toInt());
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    QCOMPARE(cut->workId(), 1234);
    QCOMPARE(cut->sn(), testData.value("newSn").toString());
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_sn)->toString().isEmpty(), 1);
    QCOMPARE(cut->warranty(), testData.value("newWarranty").toInt());
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_warranty)->toInt(), 93);

    QString testStr;
    QCOMPARE(cut->i_logTexts.count(), 4);
    testStr = tr("[А] Количество товара \"%1\" изменёно с %2 на %3")
              .arg(cut->name())
              .arg(countOld)
              .arg(countNew);
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    testStr = tr("Стоимость товара \"%1\" изменена с %2 на %3")
              .arg(cut->name())
              .arg(sysLocale.toCurrencyString(testData.value("price").toDouble()))
              .arg(sysLocale.toCurrencyString(testData.value("newPrice").toDouble()));
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    testStr = tr("Серийный номер товара \"%1\" установлен \"%2\"")
              .arg(cut->name())
              .arg(testData.value("newSn").toString());
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);
    testStr = tr("Срок гарантии на товар \"%1\" изменён с \"%2\" на \"%3\"")
              .arg(cut->name())
              .arg(warrantyTermsModel->getDisplayRole(testData.value("warranty").toInt(), 1))
              .arg(warrantyTermsModel->getDisplayRole(testData.value("newWarranty").toInt(), 1));
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);


    QCOMPARE(cut->cache.count(), 4);    // для неизменённых строк объект SRepairSaleItemModel не должен создаваться, но проверяем на всякий случай

    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->m_storeItem->m_commitedExpense, countOld);
    QCOMPARE(cut->m_storeItem->m_lastOp, -1);    // если изменяется кол-во, то в табл. store_items производятся изменения
    QCOMPARE(cut->m_storeItem->m_commited, 0);

}

/* Удаление товара из списка работ и деталей (обычный ремонт) */
void TClassTest::test_loadBoqAndRemoveItem()
{
//    QSKIP("");
    int id = testData.value("id").toInt();
    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SRepairSaleItemModel::C_repairId: strct->data = testData.value("repair").toInt(); break;
            case SRepairSaleItemModel::C_workId: strct->data = 1234; break;
            case SRepairSaleItemModel::C_state: strct->data = SRepairSaleItemModel::RepairLinked; break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);
    cut->set_state(SRepairSaleItemModel::EngineerBasket);

    QString testStr;
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    testStr = tr("Удален товар \"%1\" стоимостью %2 в кол-ве %3ед.")
              .arg(cut->name()).arg(sysLocale.toCurrencyString(cut->price()))
              .arg(cut->count());
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->unlinkRepair();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->data(SRepairSaleItemModel::C_state), SRepairSaleItemModel::EngineerBasket);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::EngineerBasket);
    QCOMPARE(cut->repairId(), 0);
    QCOMPARE(cut->workId(), 0);
    QCOMPARE(cut->i_logRecord->cache[SLogRecordModel::C_id]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->i_logTexts.count(), 0);
    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->m_storeItem->m_lastOp, -1);   // при удалении товара из обычного ремонта, он остаётся в корзине сотрудника; в таблице store_items ничего не зименяется
    QCOMPARE(cut->m_storeItem->m_commited, 0);

}

/* Удаление товара из списка работ и деталей (картридж)
 * Идентичен тесту для обычного ремонта, но запись сразу удаляется из корзины сотрудника
*/
void TClassTest::test_loadBoqAndRemoveItem2()
{
//    QSKIP("");
    int id = testData.value("id").toInt();
    auto itemSubstitute = [&](int index, InitStruct *strct){
        switch(index)
        {
            case SRepairSaleItemModel::C_repairId: strct->data = testData.value("repair").toInt(); break;
            case SRepairSaleItemModel::C_workId: strct->data = 1234; break;
            case SRepairSaleItemModel::C_state: strct->data = SRepairSaleItemModel::RepairLinked; break;
            default: ;
        }
    };
    connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
    auto cut = prepareModel(id);
    disconnect(this);
    cut->set_state(SRepairSaleItemModel::Archive);

    QString testStr;
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    QCOMPARE(cut->i_logTexts.count(), 1);
    testStr = tr("Удален товар \"%1\" стоимостью %2 в кол-ве %3ед.")
              .arg(cut->name()).arg(sysLocale.toCurrencyString(cut->price()))
              .arg(cut->count());
    QCOMPARE(cut->i_logTexts.values().contains(testStr), 1);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->unlinkRepair();
    cut->free();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->data(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Archive);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Archive);
    QCOMPARE(cut->repairId(), 0);
    QCOMPARE(cut->workId(), 0);
    QCOMPARE(cut->i_logRecord->cache[SLogRecordModel::C_id]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->i_logTexts.count(), 0);
    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->m_storeItem->m_lastOp, 4);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

void TClassTest::test_loadBoqAndIssue()
{
    //    QSKIP("");
        int id = testData.value("id").toInt();
        auto itemSubstitute = [&](int index, InitStruct *strct){
            switch(index)
            {
                case SRepairSaleItemModel::C_repairId: strct->data = testData.value("repair").toInt(); break;
                case SRepairSaleItemModel::C_workId: strct->data = 1234; break;
                default: ;
            }
        };
        connect(this, &TClassTest::itemInitSubstitute, itemSubstitute);
        auto cut = prepareModel(id);
        disconnect(this);

        QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
        QCOMPARE(cut->i_logTexts.count(), 0);

        auto db = QSqlDatabase::database(TdConn::session());
        auto query = std::make_unique<QSqlQuery>(db);
        debugStuff->startSqlLog(db, __func__);
        query->exec(QString("# =========================================== %1").arg(__func__));
        query->exec("BEGIN;");

        cut->sale();
        query->exec("ROLLBACK;");
        QCOMPARE(cut->data(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Sold);
        QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Sold);
        QCOMPARE(cut->repairId(), testData.value("repair").toInt());
        QCOMPARE(cut->workId(), 1234);
        QCOMPARE(cut->i_logRecord->cache.contains(SRepairSaleItemModel::C_notes), 0);
        QCOMPARE(cut->i_logTexts.count(), 0);
        QVERIFY(cut->m_storeItem != nullptr);
        QCOMPARE(cut->m_storeItem->m_lastOp, 2);
        QCOMPARE(cut->m_storeItem->m_commited, 1);
}

/* Добавление детали в корзину сотрудника командой контекстного меню Выдать товар
 * Создание и инициализация новой модели могут быть выполнены:
 *  - в обработчике команды меню; указатель на объект передаётся в качестве параметра в контруктор класса вкладки
 *  - в контрукторе класса вкладки; данные передаются в контруктор в виде ссылки на QSqlRecord
 */
void TClassTest::test_addItemToBasket()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_shared<SRepairSaleItemModel>();

    QVERIFY(cut->m_storeItem != nullptr);
    cut->set_name(testData.value("itemName").toString());
    cut->set_itemId(testData.value("itemId").toInt());
    cut->set_warranty(testData.value("warranty").toInt());

    QCOMPARE(cut->isPrimaryKeyValid(), false);
    QCOMPARE(cut->isModelLoaded(), false);
    QCOMPARE(cut->itemId(), testData.value("itemId").toInt());
    QCOMPARE(cut->name(), testData.value("itemName").toString());
    QCOMPARE(cut->price(), 0);
    QCOMPARE(cut->count(), 0);
    QCOMPARE(cut->notes(), "");
    QCOMPARE(cut->repairId(), 0);
    QCOMPARE(cut->i_logRecord->repair(), 0);
    QCOMPARE(cut->workId(), 0);
    QCOMPARE(cut->sn(), "");
    QCOMPARE(cut->warranty(), testData.value("warranty").toInt());
    QCOMPARE(cut->cache.count(), 3);

    // TODO: при добавлении в корзину отсутствует номер ремонта, записей в журнал не должно быть, т. к. они будут "бесхозные"
    QCOMPARE(cut->i_logTexts.count(), 0);

    // следующие действия будут выполнены в методе beginCommit() вкладки:
    cut->set_fromUser(userDbData->id());
    cut->set_toUser(1);
    cut->set_price(testData.value("newPrice").toDouble());
    cut->set_count(1);
    cut->set_sn(testData.value("newSn").toString());

    QCOMPARE(cut->notes(), tr("Выдача %1ед. товара сотруднику %2").arg(cut->count()).arg(allUsersModel->getDisplayRole(cut->toUser())));
    QCOMPARE(cut->cache.count(), 9);
    QCOMPARE(cut->m_storeItem->m_newExpense, 1);

    // TODO: при добавлении в корзину отсутствует номер ремонта, записей в журнал не должно быть, т. к. они будут "бесхозные"
    QCOMPARE(cut->i_logTexts.count(), 0);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->set_state(SRepairSaleItemModel::EngineerBasket); // установка статуса происходит в момент нажатия кнопки "Выдать" на вкладке управления запросом детали со склада
    cut->commit();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->state(), SRepairSaleItemModel::EngineerBasket);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::EngineerBasket);
    QCOMPARE(cut->m_storeItem->m_commitedExpense, 1);
    QCOMPARE(cut->m_storeItem->m_lastOp, 1);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

/* Обработка ранее созданной заявки на деталь со склада
 * Модель иницилизируется вызовом метода load()
 */
void TClassTest::test_addItemToBasket2()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_shared<SRepairSaleItemModel>();

    // это как бы load():
    cut->m_record->setValue(SRepairSaleItemModel::C_id, 4586);
    cut->m_record->setValue(SRepairSaleItemModel::C_itemId, testData.value("itemId").toInt());
    cut->m_record->setValue(SRepairSaleItemModel::C_name, testData.value("itemName").toString());
    cut->m_record->setValue(SRepairSaleItemModel::C_count, 1);
    cut->m_storeItem->setCommitedExpense(cut->count());
    cut->m_storeItem->setNewExpense(cut->count());
    cut->m_record->setValue(SRepairSaleItemModel::C_fromUser, userDbData->id());
    cut->m_record->setValue(SRepairSaleItemModel::C_toUser, 1);
    cut->m_record->setValue(SRepairSaleItemModel::C_notes, tr("Выдача %1ед. товара сотруднику %2").arg(cut->count()).arg(allUsersMap->value(userDbData->id())));
    cut->m_record->setValue(SRepairSaleItemModel::C_price, testData.value("newPrice").toDouble());
    cut->m_record->setValue(SRepairSaleItemModel::C_sn, testData.value("newSn").toString());
    cut->m_record->setValue(SRepairSaleItemModel::C_warranty, testData.value("warranty").toInt());
    cut->m_record->setValue(SRepairSaleItemModel::C_state, SRepairSaleItemModel::Requested);

    // TODO: при добавлении в корзину отсутствует номер ремонта, записей в журнал не должно быть, т. к. они будут "бесхозные"
    QCOMPARE(cut->i_logTexts.count(), 0);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->set_state(SRepairSaleItemModel::EngineerBasket); // установка статуса происходит в момент нажатия кнопки "Выдать" на вкладке управления запросом детали со склада
    cut->commit();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->state(), SRepairSaleItemModel::EngineerBasket);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::EngineerBasket);
    QCOMPARE(cut->m_storeItem->m_commitedExpense, 1);
    QCOMPARE(cut->m_storeItem->m_lastOp, 1);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
}

/* Создание записи в store_int_reserve при вызове команды контекстного меню Запросить деталь
 */
void TClassTest::test_requestItem()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_shared<SRepairSaleItemModel>();

    cut->set_name("Контакт штыревой для вилок BLS/BLD, шаг 2,54мм");
    cut->set_itemId(testData.value("itemId").toInt());
    cut->set_warranty(testData.value("warranty").toInt());
    cut->set_fromUser(userDbData->id());
    cut->set_toUser(1);
    cut->set_price(testData.value("newPrice").toDouble());
    cut->set_count(1);
    cut->set_sn(testData.value("newSn").toString());

    QCOMPARE(cut->notes(), tr("Выдача %1ед. товара сотруднику %2").arg(cut->count()).arg(allUsersModel->getDisplayRole(cut->toUser())));
    // TODO: при добавлении в корзину отсутствует номер ремонта, записей в журнал не должно быть, т. к. они будут "бесхозные"
    QCOMPARE(cut->i_logTexts.count(), 0);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->request();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->state(), SRepairSaleItemModel::Requested);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Requested);
    QCOMPARE(cut->m_storeItem->m_lastOp, -1);
    QCOMPARE(cut->m_storeItem->m_commited, 0);
}

void TClassTest::test_returnItemToWarehouse()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_shared<SRepairSaleItemModel>();

    cut->setPrimaryKey(testData.value("id"));
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    cut->load();

    QCOMPARE(cut->data(SRepairSaleItemModel::C_id), testData.value("id"));
    QCOMPARE(cut->data(SRepairSaleItemModel::C_itemId), testData.value("itemId"));
    QCOMPARE(cut->data(SRepairSaleItemModel::C_state), testData.value("state"));

    query->exec("BEGIN;");

    cut->free();
    query->exec("ROLLBACK;");
    QCOMPARE(cut->data(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Archive);
    QCOMPARE(cut->commitedData(SRepairSaleItemModel::C_state), SRepairSaleItemModel::Archive);
    QVERIFY(cut->m_storeItem != nullptr);
    QCOMPARE(cut->m_storeItem->m_lastOp, 4);
    QCOMPARE(cut->m_storeItem->m_commited, 1);
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
