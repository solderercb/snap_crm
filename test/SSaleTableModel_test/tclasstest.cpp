#include <windows.h>
#include <QCoreApplication>
#include <QTextCodec>
#include <QObject>
#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include <ProjectGlobals>
#include <memory>
#include <QDate>
#include <QDateTime>
#include <QTimeZone>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlField>
#include <SStandardItemModel>
#include <SSaleTableModel>
#include <SLogRecordModel>
#include <FlashPopup>

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
    std::unique_ptr<WorkshopSaleModel> workshopModel_with_work_and_item();
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_demo();
    void test_insertOneRow();
    void test_setData();
    void test_data();
    void test_insertRows();
    void test_check_before_item_add();
    void test_store_add_item();
    void test_removeUncommitedRow();
    void test_reportFields();
    void test_store_index_flags();
    void test_store_sale();
    void test_store_load();
    void test_store_mark_row_remove_and_unmark();
    void test_store_unsale();
    void test_store_signal_amount_changed();
    void test_store_price_col_change();
    void test_workshop_work_flags();
    void test_workshop_item_flags();
    void test_workshop_add_custom_work();
    void test_workshop_add_work_from_pricelist();
    void test_workshop_add_item();
    void test_workshop_add_item_from_basket();
    void test_workshop_removeUncommitedRow();
    void test_workshop_sale();
    void test_workshop_unsale();
    void test_workshop_markRemoveItem();
    void test_workshop_markRemoveWork();
    void test_workshop_restoreMarkedRemoveItem();
    void test_workshop_removeWork();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

void TClassTest::initAuxiliaryModels()
{
    allUsersMap->insert(1, "test1");

    QVector<QString> warrantyTermsList = {QObject::tr("нет"), QObject::tr("7 дней"), QObject::tr("14 дней"), QObject::tr("1 мес"), QObject::tr("2 мес"), QObject::tr("3 мес"), QObject::tr("4 мес"), QObject::tr("6 мес"), QObject::tr("1 год"), QObject::tr("2 года"), QObject::tr("3 года")};
    int warrantyTerms[] = {0, 7, 14, 31, 62, 93, 124, 186, 365, 730, 1095};
    // Модель для комбобоксов при редактировании, а QMap для отображения
    for (int i=0; i<warrantyTermsList.size(); i++)
    {
        QList<QStandardItem*> *warrantyTermSelector = new QList<QStandardItem*>();
        *warrantyTermSelector << new QStandardItem(warrantyTermsList.at(i)) << new QStandardItem(QString::number(warrantyTerms[i]));
        warrantyTermsModel->appendRow(*warrantyTermSelector);
//        warrantyTermsMap->insert(warrantyTerms[i], warrantyTermsList.at(i));
    }
    warrantyTermsModel->setObjectName("warrantyTermsModel");
    warrantyTermsModel->setHorizontalHeaderLabels({"name","days"});

    QVector<QString> priceColDBFieldsList = {"`price`", "`price2`", "`price3`", "`price4`", "`price5`", "0"};
    for (int i=0; i<priceColDBFieldsList.size(); i++)
    {
        QList<QStandardItem*> *priceColSelector = new QList<QStandardItem*>();
        *priceColSelector << new QStandardItem(QString::number(i)) << new QStandardItem(QString::number(i)) << new QStandardItem(priceColDBFieldsList[i]);
        priceColModel->appendRow(*priceColSelector);
    }

    priceColModel->setObjectName("priceColModel");
    priceColModel->setHorizontalHeaderLabels({"name", "id", "dbColumn"});

    QVector<QString> workPriceColDBFieldsList = {"`price1`", "`price2`", "0"};
    for (int i=0; i<workPriceColDBFieldsList.size(); i++)
    {
        QList<QStandardItem*> *workPriceColSelector = new QList<QStandardItem*>();
        *workPriceColSelector << new QStandardItem(QString::number(i)) << new QStandardItem(QString::number(i)) << new QStandardItem(workPriceColDBFieldsList[i]);
        workPriceColModel->appendRow(*workPriceColSelector);
    }

    workPriceColModel->setObjectName("workPriceColModel");
    workPriceColModel->setHorizontalHeaderLabels({"name", "id", "dbColumn"});
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
    SetConsoleOutputCP(65001);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("SSaleTableModel_test", testData);
}

void TClassTest::cleanupTestCase()
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connMain"));
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("ROLLBACK;");
    query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("ROLLBACK;");
}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database("connMain").isOpen() == 1);
//    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
    initAuxiliaryModels();
}

void TClassTest::test_newModel()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    QCOMPARE(cut->rowCount(), 0);
    QCOMPARE(cut->columnCount(), 24);
    QCOMPARE(cut->m_modified.count(), 0);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_demo()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    cut->initDemo();
    QCOMPARE(cut->rowCount(), 3);
    QCOMPARE(cut->columnCount(), 24);
    QCOMPARE(cut->m_modified.count(), 3);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_insertOneRow()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();

    cut->QStandardItemModel::insertRows(0, 1);
    QCOMPARE(cut->rowCount(), 1);
    QCOMPARE(cut->columnCount(), SSaleTableModel::Columns::WorkType + 1);
    QCOMPARE(cut->m_modified.count(), 0);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_setData()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    cut->QStandardItemModel::insertRows(0, 1);
    cut->QStandardItemModel::setData(cut->index(0, SSaleTableModel::Columns::RecordType), SSaleTableModel::RecordType::Item);

    QCOMPARE(cut->recordType(0), SSaleTableModel::RecordType::Item);
    auto idx = cut->index(0,0);
    QCOMPARE(idx.row(), 0);
    QCOMPARE(idx.column(), 0);
    cut->setData(idx, 0xAA);
    QCOMPARE(cut->m_modified.count(), 1);
//    QVERIFY(dynamic_cast<SStoreSaleItemModel*>(cut->cacheItem(0).get()) != nullptr);
//    QVERIFY(dynamic_cast<MapperForSingleRowModel*>(cut->cacheItem(0).get()) != nullptr);    // должно успешно кастоваться
//    QCOMPARE(cut->m_modified.value(0)->data(0), 0xAA);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_data()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    cut->QStandardItemModel::insertRows(0, 1);

    cut->setData(cut->index(0,0), 0xAA);
    QCOMPARE(cut->index(0,0).data(), 0xAA);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_insertRows()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();

    cut->QStandardItemModel::insertRows(0, 4);
    cut->QStandardItemModel::setData(cut->index(1, SSaleTableModel::Columns::RecordType), SSaleTableModel::RecordType::Item);
    cut->setData(cut->index(1,0), 0xAA);
    QCOMPARE(cut->m_modified.count(), 1);
    QCOMPARE(cut->data(cut->index(1,0)), 0xAA);
    auto ptr1 = cut->m_modified.value(1).get();
    QVERIFY(cut->m_modified.keys().contains(1) == 1);
    cut->insertRows(1, 2);
    QCOMPARE(cut->m_modified.count(), 1);
    QVERIFY(cut->m_modified.keys().contains(3) == 1);
    QCOMPARE(cut->m_modified.value(3).get(), ptr1);

    QCOMPARE(cut->data(cut->index(3,0)), 0xAA);
    QCOMPARE(cut->index(3,0).data(), 0xAA);


//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_check_before_item_add()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    int popupCount = shortlivedNotification::p_instance.count();

    if(cut->SSaleTableModel::checkBeforeItemAdd(testData.value("itemId").toInt(), 1) == 0) // общие проверки
    {
        QVERIFY(shortlivedNotification::p_instance.count() > popupCount);
        QFAIL("SSaleTableModel::checkBeforeItemAdd() returned 0, 1 expected");
    }

    cut->dbgAddRandomItems(1);
    int item_id = cut->index(0, SSaleTableModel::Columns::ItemId).data().toInt();
    int item_avail = cut->index(0, SSaleTableModel::Columns::Avail).data().toInt();
    QVERIFY(item_avail > 0);
    QCOMPARE(cut->checkBeforeItemAdd(item_id, 1), 0); // проверка повтора: в режиме простой продажи нельзя добавить один и тот же товар дважды
    QCOMPARE(cut->SSaleTableModel::checkBeforeItemAdd(item_id, item_avail + 1), 0); // проверка превышения запрошенного кол-ва над доступным

    auto cut2 = std::make_unique<WorkshopSaleModel>();
    cut2->setEditStrategy(WorkshopSaleModel::EditStrategy::OnManualSubmit);
    cut2->addItemByID(item_id);
    QCOMPARE(cut2->checkBeforeItemAdd(item_id, 1), 1); // проверка повтора: в ремонте можно добавить один и тот же товар сколько угодно раз
}

void TClassTest::test_store_add_item()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    int added = 0;

    cut->QStandardItemModel::insertRows(0, 4);
    QCOMPARE(cut->rowCount(), 4);
    QCOMPARE(cut->m_modified.count(), 0);

    added = cut->dbgAddRandomItems(1);
    QVERIFY(added > 0);
    QCOMPARE(cut->m_modified.count(), 1);
    QVERIFY(cut->m_modified.keys().contains(4) == 1);
    QVERIFY(cut->m_modified.keys().contains(2) == 0);
    auto rec = cut->cacheItem(4);
    QVERIFY(static_cast<SStoreSaleItemModel*>(rec.get())->m_storeItem->id() != 0);
    QVERIFY(rec->isPrimaryKeyValid() == false);
    QVERIFY(rec->isDirty() == true);
    foreach(auto f, rec->cache)
    {
//        auto key = rec->cache.key(f);
//        qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(key) << DBG_VAR_OUT(rec->fieldName(key)) << DBG_VAR_OUT(rec->data(key)) ;
        QVERIFY(f->state() == SSingleRowModelBase::ModifiedField::Updated);
    }
    QVERIFY(cut->index(4, SSaleTableModel::Columns::UID).data().toString().isEmpty() == 0);
    QVERIFY(cut->index(4, SSaleTableModel::Columns::Name).data().toString().isEmpty() == 0);

    added = cut->dbgAddRandomItems(1);
    QVERIFY(added > 0);
    cut->insertRows(3, 1);
    QCOMPARE(cut->m_modified.count(), 2);
    QCOMPARE(cut->rowCount(), 7);
    auto keys = cut->m_modified.keys();
    QVERIFY(keys.contains(4) == 0);
    QVERIFY(keys.contains(5) == 1);
    QVERIFY(keys.contains(6) == 1);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

void TClassTest::test_removeUncommitedRow()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();

    cut->QStandardItemModel::insertRows(0, 4);
    cut->QStandardItemModel::setData(cut->index(3, SSaleTableModel::Columns::RecordType), SSaleTableModel::RecordType::Item);
    cut->setData(cut->index(3,0), 0xAA);
    auto ptr3 = cut->m_modified.value(3).get();
    cut->removeRows(1, 1);
    QCOMPARE(cut->m_modified.count(), 1);
    QVERIFY(cut->m_modified.keys().contains(2) == 1);
    QCOMPARE(cut->m_modified.value(2).get(), ptr3);
    QCOMPARE(cut->data(cut->index(2,0)), 0xAA);

//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
//    QCOMPARE(cut, );
}

/* Проверка сооответствия названий и кол-ва полей для отчетов.
 * Желательно, чтобы при изменении модели поля не изменялись, чтобы не переделывать отчеты. Этот тест выявит различия.
*/
void TClassTest::test_reportFields()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    cut->initDemo();
    LimeReport::CallbackInfo repInfo;
    QVariant repData;

    repInfo.dataType = LimeReport::CallbackInfo::ColumnCount;
    cut->reportCallbackData(repInfo, repData);
    int columnCount = repData.toInt();
    // Список всех полей, доступных в отчете, до версии 0.0.0.304 (включительно)
    QStringList fields = {"UID", "id", "item", "qty", "price", "summ", "serialNumber", "warranty", "performer", "worksAmount", "itemsAmount", "amount", "amountStr"};

    for(int i = 0; i < columnCount; i++)
    {
        QVERIFY(fields.count() != 0);
        repInfo.dataType = LimeReport::CallbackInfo::ColumnHeaderData;
        repInfo.index = i;
        cut->reportCallbackData(repInfo, repData);
        fields.removeOne(repData.toString());
    }

    if(fields.count() != 0)
    {
        qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(fields);
        QVERIFY(0);
    }
}

void TClassTest::test_store_index_flags()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    cut->QStandardItemModel::insertRows(0, 1);
    cut->QStandardItemModel::setData(cut->index(0, SSaleTableModel::Columns::RecordType), SSaleTableModel::RecordType::Item);

    for(int i = 0; i < cut->columnCount(); i++)
    {
        Qt::ItemFlags flags;
        switch (i)
        {
            case SSaleTableModel::Columns::Count:
            case SSaleTableModel::Columns::Price:
            case SSaleTableModel::Columns::SN:
            case SSaleTableModel::Columns::Warranty:
            case SSaleTableModel::Columns::User: flags = (Qt::ItemIsEnabled | Qt::ItemIsEditable); break;
            default: flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable); break;
        }
        QVERIFY(cut->flags(cut->index(0, i)) == flags);
    }

    cut->setState(StoreSaleModel::State::Sold);
    for(int i = 0; i < cut->columnCount(); i++)
    {
        QVERIFY(cut->flags(cut->index(0, i)) == (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    }
}

void TClassTest::test_store_sale()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);

    cut->setRowCount(0);
    cut->dbgAddRandomItems(2);
    cut->setData(cut->index(0, SSaleTableModel::Columns::Warranty), testData.value("warranty").toInt());
    QCOMPARE(cut->rowCount(), 2);
    QCOMPARE(cut->m_modified.count(), 2);
    cut->setDocumentId(2);
    cut->setClient(testData.value("customer").toInt());

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit(StoreSaleModel::OpType::Sale);
    auto rec = static_cast<SStoreSaleItemModel*>(cut->cacheItem(0).get());
    QCOMPARE(rec->m_storeItem->m_lastOp, 5);

    query->exec("ROLLBACK;");
}

void TClassTest::test_store_load()
{
//    QSKIP("");
    auto db = QSqlDatabase::database("connMain");
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<StoreSaleModel>();
    cut->setState(StoreSaleModel::Sold);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    cut->loadTable(testData.value("commitedDocId").toInt());

    QCOMPARE(cut->rowCount(), testData.value("commitedDocRows").toInt());
    QCOMPARE(cut->columnCount(), SSaleTableModel::Columns::WorkType + 1);
}

void TClassTest::test_store_mark_row_remove_and_unmark()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    cut->setState(StoreSaleModel::Sold);
    cut->loadTable(testData.value("commitedDocId").toInt());

    int row = 1;
    int loadedCount = cut->index(1, SSaleTableModel::Columns::Count).data().toInt();
    cut->markRowRemove(row);
    QCOMPARE(cut->index(1, SSaleTableModel::Columns::State).data().toInt(), SStoreSaleItemModel::State::Cancelled);
    QCOMPARE(cut->index(1, SSaleTableModel::Columns::Count).data().toInt(), loadedCount);

    cut->markRowRemove(row);
    QCOMPARE(cut->index(1, SSaleTableModel::Columns::State).data().toInt(), SStoreSaleItemModel::State::Active);
    QCOMPARE(cut->index(1, SSaleTableModel::Columns::Count).data().toInt(), loadedCount);
}

void TClassTest::test_store_unsale()
{
//    QSKIP("");
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<StoreSaleModel>();
    cut->setConnection(db);
    cut->setState(StoreSaleModel::Sold);
    cut->loadTable(testData.value("commitedDocId").toInt());

    int row = 1;
    cut->markRowRemove(row);
    QCOMPARE(cut->m_modified.count(), 1);
    QVERIFY(cut->cacheItem(row)->isDirty() == true);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->unsaleItems("TClassTest::test_store_unsale()");
    QVERIFY(cut->cacheItem(row)->isDirty() == false);
    cut->loadTable(testData.value("commitedDocId").toInt());
    QCOMPARE(cut->rowCount(), 3);
    QCOMPARE(cut->m_modified.count(), 0);
    QCOMPARE(cut->index(1, SSaleTableModel::Columns::State).data().toInt(), SStoreSaleItemModel::State::Cancelled);
    QCOMPARE(cut->index(1, SSaleTableModel::Columns::Count).data().toInt(), 0);
    query->exec("ROLLBACK;");
}

void TClassTest::test_store_signal_amount_changed()
{
//    QSKIP("");
    int signalCounter = 0;
    double amountTotal = 0, amountItems = 0, amountWorks = 0;

    auto slot = [&](double am_tot, double am_it, double am_wk){
        signalCounter++;
        amountTotal = am_tot;
        amountItems = am_it;
        amountWorks = am_wk;
    };

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    auto cut = std::make_unique<StoreSaleModel>();
    connect(cut.get(), &StoreSaleModel::amountChanged, this, slot);
    cut->setConnection(db);
    cut->setState(StoreSaleModel::Sold);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    cut->loadTable(testData.value("commitedDocId").toInt());
    cut->setState(StoreSaleModel::Reserved);    // чтобы методы в StoreSaleModel работали как в случае Резерва

    QCOMPARE(signalCounter, 1);
    QCOMPARE(amountTotal, testData.value("commitedDocTotal").toDouble());
    QCOMPARE(amountWorks, 0.00);
    QVERIFY(amountItems == amountTotal);

    cut->dbgAddRandomItems(2);
    QVERIFY(cut->rowCount() == 5);
    QVERIFY(signalCounter == 3);
    QVERIFY(amountTotal > testData.value("commitedDocTotal").toDouble());
    double updatedAmount = amountTotal;
    QCOMPARE(amountWorks, 0.00);
    QVERIFY(amountItems == amountTotal);

    cut->markRowRemove(2);
    QCOMPARE(signalCounter, 4);
    QVERIFY(amountTotal < updatedAmount);
    QCOMPARE(amountWorks, 0.00);
    QVERIFY(amountItems == amountTotal);
}

void TClassTest::test_store_price_col_change()
{
//    QSKIP("");
    auto cut = std::make_unique<StoreSaleModel>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);

    cut->setRowCount(0);
    cut->addItemByID(testData.value("itemId").toInt());
    double price = cut->SStandardItemModel::data(cut->index(0, SSaleTableModel::Columns::Price)).toDouble();
    double item_amount = cut->SStandardItemModel::data(cut->index(0, SSaleTableModel::Columns::Amount)).toDouble();
    double total_amount = cut->amountTotal();
    QCOMPARE(cut->SStandardItemModel::data(cut->index(0, SSaleTableModel::Columns::Price)).toDouble(), price);

    cut->setPriceColumn(SStoreItemModel::PriceOptionService);
    QVERIFY(cut->SStandardItemModel::data(cut->index(0, SSaleTableModel::Columns::Price)).toDouble() != price);
    QVERIFY(cut->amountTotal() != total_amount);
}

void TClassTest::test_workshop_work_flags()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    cut->QStandardItemModel::insertRows(0, 1);
    cut->QStandardItemModel::setData(cut->index(0, SSaleTableModel::Columns::RecordType), SSaleTableModel::RecordType::Work);

    Qt::ItemFlags flags;

    cut->setState(WorkshopSaleModel::State::RW);
    for(int i = 0; i < cut->columnCount(); i++) // проверка флагов полей строки кастомной работы в режиме RW
    {
        switch (i)
        {
            case SSaleTableModel::Columns::Name:
            case SSaleTableModel::Columns::Price:
            case SSaleTableModel::Columns::Count:
            case SSaleTableModel::Columns::Warranty:
            case SSaleTableModel::Columns::User: flags = (Qt::ItemIsEnabled | Qt::ItemIsEditable); break;
            default: flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable); break;
        }
        QVERIFY(cut->flags(cut->index(0, i)) == flags);
    }

    cut->QStandardItemModel::setData(cut->index(0, SSaleTableModel::Columns::UID), "1111"); // если работа из прайс-листа, то изменять её стоимость нельзя
    QVERIFY(cut->flags(cut->index(0, SSaleTableModel::Columns::Price)) == (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    cut->setState(WorkshopSaleModel::State::Adm); // в режиме админ. правки можно изменять стоимость работы даже если она из прайс-листа
    QVERIFY(cut->flags(cut->index(0, SSaleTableModel::Columns::Price)) == (Qt::ItemIsEnabled | Qt::ItemIsEditable));

    cut->setState(WorkshopSaleModel::State::RO);
    for(int i = 0; i < cut->columnCount(); i++)
    {
        QVERIFY(cut->flags(cut->index(0, i)) == (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    }
}

void TClassTest::test_workshop_item_flags()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    cut->QStandardItemModel::insertRows(0, 1);
    cut->QStandardItemModel::setData(cut->index(0, SSaleTableModel::Columns::RecordType), SSaleTableModel::RecordType::Item);

    Qt::ItemFlags flags;
    for(int i = 0; i < cut->columnCount(); i++) // в режиме RW в строке товара можно редактировать только два поля
    {
        switch (i)
        {
            case SSaleTableModel::Columns::SN:
            case SSaleTableModel::Columns::Warranty: flags = (Qt::ItemIsEnabled | Qt::ItemIsEditable); break;
            default: flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable); break;
        }
        QVERIFY(cut->flags(cut->index(0, i)) == flags);
    }

    cut->setState(WorkshopSaleModel::State::Adm);
    for(int i = 0; i < cut->columnCount(); i++) // в режиме админ. правки в строке товара дополнительно можно редактировать кол-во, цену и пользователя
    {
        switch (i)
        {
            case SSaleTableModel::Columns::Count:
            case SSaleTableModel::Columns::Price:
            case SSaleTableModel::Columns::SN:
            case SSaleTableModel::Columns::Warranty:
            case SSaleTableModel::Columns::User: flags = (Qt::ItemIsEnabled | Qt::ItemIsEditable); break;
            default: flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable); break;
        }
        QVERIFY(cut->flags(cut->index(0, i)) == flags);
    }

    permissions->addGoodsFromWarehouse = 0; // если по какой-то причине у пользователя есть право админ правки списка, но нет права добавлять детали в ремонт, то изменять кол-во нельзя
    flags = cut->flags(cut->index(0, SSaleTableModel::Columns::Count));
    permissions->addGoodsFromWarehouse = 1;
    QVERIFY(flags == (Qt::ItemIsEnabled | Qt::ItemIsSelectable));

    cut->setState(WorkshopSaleModel::State::RO);
    for(int i = 0; i < cut->columnCount(); i++)
    {
        QVERIFY(cut->flags(cut->index(0, i)) == (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    }
}

void TClassTest::test_workshop_add_custom_work()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    cut->setEditStrategy(WorkshopSaleModel::EditStrategy::OnManualSubmit);

    cut->setRepairId(2);
    cut->addCustomWork();
    QCOMPARE(cut->m_modified.count(), 1);

}

void TClassTest::test_workshop_add_work_from_pricelist()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    cut->setEditStrategy(WorkshopSaleModel::EditStrategy::OnManualSubmit);
    cut->setRepairId(2);

    QVERIFY(cut->addWorkByID(testData.value("workPriceListId").toInt(), 0) == 1);
    QCOMPARE(cut->m_modified.count(), 1);
    auto work = static_cast<SWorkModel*>(cut->cacheItem(0).get());
    QVERIFY(work->price() == testData.value("workPriceListPrice").toDouble());
    QCOMPARE(work->payRepair(), testData.value("workPriceListEngineerRoyalty").toInt());
    QCOMPARE(work->payRepairQuick(), testData.value("workPriceListEngineerRoyalty").toInt());
    cut->setData(cut->index(0, SSaleTableModel::Columns::User), 2);
    QCOMPARE(work->payRepair(), testData.value("workPriceListEngineerRoyalty").toInt()); // для работы из прайс-листа процент заработка инженера не должен изменяться
}

/* Возвращает указатель на модель с одной работой и одним товаром
 * Используется далее в нескольких тестах
*/
std::unique_ptr<WorkshopSaleModel> TClassTest::workshopModel_with_work_and_item()
{
    auto cut = std::make_unique<WorkshopSaleModel>();
    cut->setEditStrategy(WorkshopSaleModel::EditStrategy::OnManualSubmit);

    cut->setRepairId(2);
    cut->dbgAddRandomItems(1);

    return cut;
}

void TClassTest::test_workshop_add_item()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();

    QCOMPARE(cut->rowCount(), 2); // при добавлении детали в пустую таблицу должна добавться работа
    cut->setData(cut->index(0, SSaleTableModel::Columns::Warranty), testData.value("warranty").toInt());
    QCOMPARE(cut->m_modified.count(), 2);
    auto work = static_cast<SWorkModel*>(cut->cacheItem(0).get());
    auto item = static_cast<SRepairSaleItemModel*>(cut->cacheItem(1).get());

    QVERIFY(work->isDirty() == 1);
    QVERIFY(item->isDirty() == 1);
    QVERIFY(item->m_storeItem->id() != 0);
}

void TClassTest::test_workshop_add_item_from_basket()
{
//    QSKIP("");
    auto cut = std::make_unique<WorkshopSaleModel>();
    cut->setEditStrategy(WorkshopSaleModel::EditStrategy::OnManualSubmit);
    cut->setRepairId(2);

    cut->addItemFromBasket(testData.value("basketItemId").toInt());
    QCOMPARE(cut->rowCount(), 2); // при добавлении детали в пустую таблицу должна добавться работа
    cut->setData(cut->index(0, SSaleTableModel::Columns::Warranty), testData.value("warranty").toInt());
    QCOMPARE(cut->m_modified.count(), 2);
    auto work = static_cast<SWorkModel*>(cut->cacheItem(0).get());
    auto item = static_cast<SRepairSaleItemModel*>(cut->cacheItem(1).get());

    QVERIFY(work->isDirty() == 1);
    QVERIFY(item->isDirty() == 1);
    QVERIFY(item->m_storeItem->id() != 0);
}

void TClassTest::test_workshop_removeUncommitedRow()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    cut->removeRow(1);  // удаление товара
    QCOMPARE(cut->rowCount(), 1);
    QCOMPARE(cut->m_modified.count(), 1);

    cut = workshopModel_with_work_and_item();
    cut->removeRow(0);  // удаление работы, все связанные товары тоже должны быть удалены
    QCOMPARE(cut->rowCount(), 0);
    QCOMPARE(cut->m_modified.count(), 0);
}

void TClassTest::test_workshop_sale()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    auto db = QSqlDatabase::database("connThird");
    cut->setConnection(db);
    auto query = std::make_unique<QSqlQuery>(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();
    auto work = static_cast<SWorkModel*>(cut->cacheItem(0).get());
    auto item = static_cast<SRepairSaleItemModel*>(cut->cacheItem(1).get());
    QVERIFY(work->isDirty() == 0);
    QVERIFY(item->isDirty() == 0);

    cut->setData(cut->index(0, SSaleTableModel::Columns::Warranty), 93);
    cut->commit(WorkshopSaleModel::OpType::Sale);  // здесь сначала сохранится таблица, потом детали будут списаны
    QCOMPARE(item->m_storeItem->m_lastOp, 2);
    QCOMPARE(item->m_storeItem->m_commited, 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_workshop_unsale()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    auto db = QSqlDatabase::database("connThird");
    cut->setConnection(db);
    auto query = std::make_unique<QSqlQuery>(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit(WorkshopSaleModel::OpType::Sale);  // сохранение и списание деталей
    cut->commit(WorkshopSaleModel::OpType::Unsale);  // возврат деталей
    auto item = static_cast<SRepairSaleItemModel*>(cut->cacheItem(1).get());
    QCOMPARE(item->m_storeItem->m_lastOp, 3);
    QCOMPARE(item->m_storeItem->m_commited, 1);

    query->exec("ROLLBACK;");
}

void TClassTest::test_workshop_markRemoveItem()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    auto db = QSqlDatabase::database("connThird");
    cut->setConnection(db);
    auto query = std::make_unique<QSqlQuery>(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();

    cut->removeRow(1);  // пометка на удаление товара
    QCOMPARE(cut->rowCount(), 2);
    QCOMPARE(cut->m_modified.count(), 2);
    auto work = static_cast<SWorkModel*>(cut->cacheItem(0).get());
    auto item = static_cast<SRepairSaleItemModel*>(cut->cacheItem(1).get());
    QVERIFY(work->isDirty() == 0);
    QVERIFY(item->isDirty() == 1);
    QVERIFY(cut->m_recordsPendingRemoveMap.count() == 1);
    QVERIFY(cut->m_recordsPendingRemoveMap.contains(item) == 1);
    QVERIFY(cut->index(1, 1).data(Qt::BackgroundRole).value<QColor>() == QColor("light gray"));
    query->exec("ROLLBACK;");
}

void TClassTest::test_workshop_markRemoveWork()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    auto db = QSqlDatabase::database("connThird");
    cut->setConnection(db);
    auto query = std::make_unique<QSqlQuery>(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();

    cut->removeRow(0);  // удаление работы, все связанные товары тоже должны быть помечены на удаление
    QCOMPARE(cut->rowCount(), 2);
    QCOMPARE(cut->m_modified.count(), 2);
    auto work = static_cast<SWorkModel*>(cut->cacheItem(0).get());
    auto item = static_cast<SRepairSaleItemModel*>(cut->cacheItem(1).get());
    QVERIFY(item->isDirty() == 1);
    QVERIFY(cut->m_recordsPendingRemoveMap.contains(item) == 1);
    QVERIFY(cut->m_recordsPendingRemoveMap.count() == 2);
    QVERIFY(work->isDirty() == 0); // запись из таблицы `works` будет удалена; поля этой записи при пометке на удаление не изменяются
    QVERIFY(cut->m_recordsPendingRemoveMap.contains(work) == 1);
    QVERIFY(cut->index(0, 1).data(Qt::BackgroundRole).value<QColor>() == QColor("light gray"));
    query->exec("ROLLBACK;");
}

void TClassTest::test_workshop_restoreMarkedRemoveItem()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    auto db = QSqlDatabase::database("connThird");
    cut->setConnection(db);
    auto query = std::make_unique<QSqlQuery>(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();

    cut->removeRow(1);  // пометка на удаление товара
    cut->removeRow(1);  // снятие пометки на удаление товара
    QVERIFY(cut->m_recordsPendingRemoveMap.count() == 0);
    QVERIFY(cut->index(1, 1).data(Qt::BackgroundRole).value<QColor>() != QColor("light gray"));
    query->exec("ROLLBACK;");
}

void TClassTest::test_workshop_removeWork()
{
//    QSKIP("");
    auto cut = workshopModel_with_work_and_item();
    auto db = QSqlDatabase::database("connThird");
    cut->setConnection(db);
    auto query = std::make_unique<QSqlQuery>(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->commit();

    cut->removeRow(0);  // удаление работы, все связанные товары тоже должны быть помечены на удаление
    auto work = cut->cacheItem(0); // во время коммита объекты удалятся из кэша, нужно их придержать
    auto item = cut->cacheItem(1);
    auto work_p = static_cast<SWorkModel*>(work.get());
    auto item_p = static_cast<SRepairSaleItemModel*>(item.get());
    QVERIFY(item_p->i_logTexts.size() == 1);
    bool item_contains_log = 0;
    foreach (auto txt, item_p->i_logTexts.values())
    {
        if(txt.contains("Удален товар"))
        {
            item_contains_log = 1;
            break;
        }
    }
    QVERIFY(item_contains_log != 0);

    cut->commit();

    query->exec("ROLLBACK;");

    QCOMPARE(work_p->i_logRecord->text().contains("Удалена работа"), 1);
    QVERIFY(item_p->m_storeItem->m_lastOp == 1);
    QVERIFY(cut->m_recordsPendingRemoveMap.count() == 0);
}

//QTEST_MAIN(TClassTest)
int main(int argc, char *argv[])
{
    TESTLIB_SELFCOVERAGE_START(TClassTest)
    QT_PREPEND_NAMESPACE(QTest::Internal::callInitMain)<TClassTest>();
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    TClassTest tc;
    QTest::setMainSourcePath(__FILE__);
    return QTest::qExec(&tc, argc, argv);
}

#include "tclasstest.moc"
