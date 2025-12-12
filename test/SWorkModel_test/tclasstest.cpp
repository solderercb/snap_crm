#include "windows.h"
#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "../../models/sworkmodel.h"
#include <SLocalSettings>
#include <ProjectGlobals>
#include <SStoreItemModel>
#include <SLogRecordModel>

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();
private:
    enum DataSet {NewWork = 0, ExistWork, ChangeNameFromEmpty, ChangeName, ChangeQty, ChangePriceFromEmpty, ChangePrice, ChangeWarrantyFromEmpty, ChangeWarranty, ChangeEngineer, ChangeNameAndPrice, ChangeEngineerAndWarranty};
    std::unique_ptr<TDebugStuff> debugStuff;
    QMap<QString, QVariant> testData;
   std::unique_ptr<SWorkModel> prepareModel(int variant) const;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_newModel2();
    void test_noChanges();
    void test_changeNameFromEmpty();
    void test_changeName();
    void test_changeQty();
    void test_changePriceFromEmpty();
    void test_changePrice();
    void test_changeEngineer();
    void test_changeWarrantyFromEmpty();
    void test_changeWarranty();
    void test_changeNameAndPrice();
    void test_changeEngineerAndWarranty();
    void test_remove();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

/* Это аналог метода SSaleTableModel::row(int), возвращающий данные для модели SWorkModel
 * С помощью variant можно выбрать из нескольких наборов данных
*/
std::unique_ptr<SWorkModel> TClassTest::prepareModel(int variant) const
{
    auto cut = std::make_unique<SWorkModel>();


    // инициализация значениями; далее некоторые из них будут изменены для проверки модели в различных сценариях
    cut->initFieldWithPrevLoaded(SWorkModel::C_id, testData.value("id"));
    cut->initFieldWithPrevLoaded(SWorkModel::C_user, testData.value("user").toInt());
    cut->initFieldWithPrevLoaded(SWorkModel::C_repair, testData.value("repair").toInt());
    cut->initFieldWithPrevLoaded(SWorkModel::C_name, testData.value("name").toString());
    cut->initFieldWithPrevLoaded(SWorkModel::C_price, testData.value("price").toDouble());
    cut->initFieldWithPrevLoaded(SWorkModel::C_count, testData.value("qty").toInt());
    cut->initFieldWithPrevLoaded(SWorkModel::C_warranty, testData.value("warranty").toInt());
    cut->initFieldWithPrevLoaded(SWorkModel::C_priceId, "0");
    cut->initFieldWithPrevLoaded(SWorkModel::C_created, QDateTime::currentDateTimeUtc());
    cut->initFieldWithPrevLoaded(SWorkModel::C_type, "0");
    cut->initFieldWithPrevLoaded(SWorkModel::C_payRepair, "0");

    // инициализированные значения переносятся в кэш, чтобы поле считалось Updated
    auto convToUpdated = [&](const int index){
        QVariant val = cut->m_record->value(index);
        if(!val.isValid())
            return;
        cut->m_record->setValue(index, QVariant());
        cut->setData(index, val);
    };

    // изменение значений к нужному сценарию
    switch (variant)
    {
        case DataSet::NewWork: cut->initFieldWithPrevLoaded(SWorkModel::C_id, QVariant()); for(int i = 1; i < cut->size(); i++){convToUpdated(i);}; break;
        case DataSet::ExistWork: break;
        case DataSet::ChangeNameFromEmpty: cut->initFieldWithPrevLoaded(SWorkModel::C_name, ""); cut->set_name(testData.value("name").toString()); break;
        case DataSet::ChangeName: cut->set_name(testData.value("name_new").toString()); break;
        case DataSet::ChangeQty: cut->set_count(testData.value("qty_new").toInt()); break;
        case DataSet::ChangePriceFromEmpty: cut->initFieldWithPrevLoaded(SWorkModel::C_price, QVariant()); cut->set_price(testData.value("price").toDouble()); break;
        case DataSet::ChangePrice: cut->set_price(testData.value("price_new").toDouble()); break;
        case DataSet::ChangeWarrantyFromEmpty: cut->initFieldWithPrevLoaded(SWorkModel::C_warranty, QVariant()); cut->set_warranty(testData.value("warranty_new").toInt()); break;
        case DataSet::ChangeWarranty: cut->set_warranty(testData.value("warranty_new").toInt()); break;
        case DataSet::ChangeEngineer: cut->set_user(testData.value("user_new").toInt()); break;
        case DataSet::ChangeNameAndPrice: cut->set_name(testData.value("name_new").toString()); cut->set_price(testData.value("price_new").toDouble()); break;
        case DataSet::ChangeEngineerAndWarranty: cut->set_user(testData.value("user_new").toInt()); cut->set_warranty(testData.value("warranty_new").toInt()); break;
        default: break;
    }

    return cut;
}

void TClassTest::initTestCase()
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF8");
//    QTextCodec::setCodecForLocale(codec);
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("sworkmodel_test", testData);
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
}

/* Это отдельный тест для ситуации добавления произвольной работы и включенного режима автосохранения.
 * Он потребовался, т. к. из-за ошибок при использовании std::optinal в release сборке основного проекта
 * происходили вылеты. И только с помощью такого варианта сбой повторялся в release сборке тестировочного
 * модуля.
*/
void TClassTest::test_newModel()
{
//    QSKIP("");

    auto cut = std::make_unique<SWorkModel>();
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    cut->set_name("");
    cut->set_count(1);
    cut->set_price(0.00);
    cut->set_warranty(0);
    cut->set_user(testData.value("user").toInt());
    cut->set_repair(testData.value("repair").toInt());
    cut->set_created(QDateTime::currentDateTimeUtc());
    cut->set_type(0);

    QCOMPARE(cut->cache.count(), 10);
    QCOMPARE(cut->i_logTexts.count(), 3);
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair"));
//    foreach (auto key, cut->i_logTexts.keys())
//    {
//        qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(key) << DBG_VAR_OUT(cut->i_logTexts[key]);
//    }
    QCOMPARE(cut->i_logTexts.values().contains(tr("Добавлена работа \"%1\" стоимостью %2")
                                                .arg("")
                                                .arg(sysLocale.toCurrencyString(0))),
             1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Исполнителем работы \"%1\" назначен %2")
                                                .arg("")
                                                .arg(usersModel->getDisplayRole(testData.value("user").toInt()))),
             1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Срок гарантии на работу \"%1\" установлен по умолчанию (\"%2\")")
                                                .arg("")
                                                .arg(warrantyTermsModel->getDisplayRole(0))),
             1);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}


void TClassTest::test_newModel2()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::NewWork);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 11);
    QCOMPARE(cut->i_logTexts.count(), 3);
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair"));
//    foreach (auto key, cut->i_logTexts.keys())
//    {
//        qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(key) << DBG_VAR_OUT(cut->i_logTexts[key]);
//    }
    QCOMPARE(cut->i_logTexts.values().contains(tr("Добавлена работа \"%1\" стоимостью %2")
                                                .arg(testData.value("name").toString())
                                                .arg(sysLocale.toCurrencyString(testData.value("price").toDouble()))),
             1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Исполнителем работы \"%1\" назначен %2")
                                                .arg(testData.value("name").toString())
                                                .arg(usersModel->getDisplayRole(testData.value("user").toInt()))),
             1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Срок гарантии на работу \"%1\" установлен по умолчанию (\"%2\")")
                                                .arg(testData.value("name").toString())
                                                .arg(warrantyTermsModel->getDisplayRole(testData.value("warranty").toInt()))),
             1);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_noChanges()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ExistWork);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logTexts.count(), 0);
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair").toInt());
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changeNameFromEmpty()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeNameFromEmpty);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Название работы изменено на \"%1\"")
                                                .arg(testData.value("name").toString())),
             1);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");

}

void TClassTest::test_changeName()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeName);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Название работы изменено с \"%1\" на \"%2\"")
                                                .arg(testData.value("name").toString())
                                                .arg(testData.value("name_new").toString())),
             1);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");

}

void TClassTest::test_changeQty()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeQty);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Кол-во \"%1\" изменёно с %2 на %3")
                                                .arg(testData.value("name").toString())
                                                .arg(testData.value("qty").toInt())
                                                .arg(testData.value("qty_new").toInt())),
             1);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changePriceFromEmpty()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangePriceFromEmpty);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    debugStuff->startSqlLog(db, __func__);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Стоимость работы \"%1\" установлена %2")
                                                .arg(testData.value("name").toString())
                                                .arg(sysLocale.toCurrencyString(testData.value("price").toDouble()))),
             1);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changePrice()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangePrice);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    debugStuff->startSqlLog(db, __func__);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Стоимость работы \"%1\" изменёна с %2 на %3")
                                                .arg(testData.value("name").toString())
                                                .arg(sysLocale.toCurrencyString(testData.value("price").toDouble()))
                                                .arg(sysLocale.toCurrencyString(testData.value("price_new").toDouble()))),
             1);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changeEngineer()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeEngineer);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 3);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Исполнитель работы \"%1\" изменён с %2 на %3")
                                                .arg(testData.value("name").toString())
                                                .arg(usersModel->getDisplayRole(testData.value("user").toInt()))
                                                .arg(usersModel->getDisplayRole(testData.value("user_new").toInt()))),
             1);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changeWarrantyFromEmpty()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeWarrantyFromEmpty);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Срок гарантии на работу \"%1\" установлен \"%2\"")
                                                .arg(testData.value("name").toString())
                                                .arg(warrantyTermsModel->getDisplayRole(testData.value("warranty_new").toInt()))),
             1);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changeWarranty()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeWarranty);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 1);
    QCOMPARE(cut->i_logTexts.count(), 1);
    QCOMPARE(cut->i_logTexts.values().contains(tr("Срок гарантии на работу \"%1\" изменён с \"%2\" на \"%3\"")
                                                .arg(testData.value("name").toString())
                                                .arg(warrantyTermsModel->getDisplayRole(testData.value("warranty").toInt()))
                                                .arg(warrantyTermsModel->getDisplayRole(testData.value("warranty_new").toInt()))),
             1);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changeNameAndPrice()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeNameAndPrice);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 2);
    QCOMPARE(cut->i_logTexts.count(), 2);
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_changeEngineerAndWarranty()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ChangeEngineerAndWarranty);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 4);
    QCOMPARE(cut->i_logTexts.count(), 2);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_remove()
{
//    QSKIP("");

    auto cut = prepareModel(DataSet::ExistWork);
    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);

    QCOMPARE(cut->cache.count(), 0);
    QCOMPARE(cut->i_logTexts.count(), 0);
    QCOMPARE(cut->i_logRecord->repair(), testData.value("repair"));
    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QVERIFY(cut->remove() == 1);
    QCOMPARE(cut->i_logRecord->text(), tr("Удалена работа \"%1\" стоимостью %2")
                                                             .arg(testData.value("name").toString())
                                                             .arg(sysLocale.toCurrencyString(testData.value("price").toDouble())));
    query->exec("ROLLBACK;");
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
