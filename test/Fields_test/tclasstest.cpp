#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "../../models/sfieldsmodel.h"
#include "../../models/ssinglerowmodel.h"
#include <SLocalSettings>
#include <ProjectQueries>
#include <FieldFactory>
#include <SFieldModel>
#include <QLineEdit>
#include <SComboBox>
#include <SDateEdit>
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
    void insertFieldsForTest(std::shared_ptr<QSqlQuery> query);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_fieldsForNewRepair();
    void test_fieldsForNewRepair_commitAfterThrow();
    void test_demo();
    void test_loadRepairFields();
    void test_loadFieldModel();
    void test_editField();
    void test_editField_commitAfterThrow();
    void test_secondAttemptToEnableEdit();
    void test_clear();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

void TClassTest::insertFieldsForTest(std::shared_ptr<QSqlQuery> query)
{
    query->exec(QUERY_NEW_ID("id", "fields"));
    if(!query->next())
        return;

    int id = 0;
    QVariant val = query->value("id");

    if(val.isNull())
    {
        if(query->value("count").toInt() == 0) // Если таблица не содержит записей
            id = 1;
    }
    else
        id = val.toInt();

    QString q1, q2;

    q1 = QString("INSERT INTO `fields` VALUES (%1, 0, 'MAC адрес сетевой карты', 1, '', 0, 0, NULL, '%2', '', 0, 0, NULL);").arg(id).arg(testData.value("device").toInt());
    q2 = QString("INSERT INTO `field_values` VALUES (NULL, %1, %2, NULL, '%3');").arg(id).arg(testData.value("repair").toInt()).arg(testData.value("mac_old").toString());
    id++;
    query->exec(q1);
    query->exec(q2);

    q1 = QString("INSERT INTO `fields` VALUES (%1, 0, 'IMEI', 1, '', 0, 1, NULL, '%2', '', 0, 0, '999999999999999;_');").arg(id).arg(testData.value("device").toInt());
    q2 = QString("INSERT INTO `field_values` VALUES (NULL, %1, %2, NULL, '%3');").arg(id).arg(testData.value("repair").toInt()).arg(testData.value("imei").toString());
    id++;
    query->exec(q1);
    query->exec(q2);

    q1 = QString("INSERT INTO `fields` VALUES (%1, 0, 'Состояние разъема ЦП', 2, 'хорошее\\r\\nнезначительные повреждения\\r\\nконтакты сильно повреждены', 1, 1, NULL, '%2', '', 0, 0, NULL);").arg(id).arg(testData.value("device").toInt());
    q2 = QString("INSERT INTO `field_values` VALUES (NULL, %1, %2, NULL, '%3');").arg(id).arg(testData.value("repair").toInt()).arg(testData.value("state").toString());
    id++;
    query->exec(q1);
    query->exec(q2);

    q1 = QString("INSERT INTO `fields` VALUES (%1, 0, 'Дата', 3, '', 0, 0, NULL, '%2', '', 0, 0, NULL);").arg(id).arg(testData.value("device").toInt());
    id++;
    query->exec(q1);

    q1 = QString("INSERT INTO `fields` VALUES (%1, 0, 'Поле для ввода целых чисел', 4, '', 0, 0, NULL, '%2', '', 0, 0, NULL);").arg(id).arg(testData.value("device").toInt());
    query->exec(q1);
}

void TClassTest::initTestCase()
{
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("repair_fields", testData);
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

void TClassTest::test_fieldsForNewRepair()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto query = std::make_shared<QSqlQuery>(db);
    std::shared_ptr<SFieldValueModel> f = nullptr;

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    insertFieldsForTest(query);


    cut->initSqlQuery(db);
    cut->init(testData.value("device").toInt());
    auto entities = cut->entitiesList();

    QCOMPARE(cut->m_valueModels.count(), 5);
    QCOMPARE(entities.count(), 5);

    // Простое текстовое поле
    auto ent = entities.at(0);
    f = cut->m_valueModels.at(0);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "QLineEdit");
    QCOMPARE(ent->widget()->metaObject()->className(), "QLineEdit");
    QCOMPARE(f->value(), QString());
    QTest::keyClicks(f->m_entityModel->widget(), testData.value("mac_new").toString());

    // Текстовое поле с валидатором
    f = cut->m_valueModels.at(1);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "QLineEdit");
    QLineEdit *le = static_cast<QLineEdit*>(f->m_entityModel->widget());
    QCOMPARE(le->inputMask(), "999999999999999;_");    // поле доложно содержать 15 цифр и не может быть пустым
    QCOMPARE(le->hasAcceptableInput(), 0);
    QTest::keyClicks(le, "test value");
    QCOMPARE(le->hasAcceptableInput(), 0);
    QTest::keyClicks(le, testData.value("imei").toString());
    QCOMPARE(le->hasAcceptableInput(), 1);

    // Выбор из списка
    f = cut->m_valueModels.at(2);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "SComboBox");
    SComboBox *cb = static_cast<SComboBox*>(f->m_entityModel->widget());
    QCOMPARE(cb->model()->rowCount(), 3);
    cb->setCurrentIndex(2);

    // Вввод даты
    f = cut->m_valueModels.at(3);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "SDateEdit");
    SDateEdit *dt = static_cast<SDateEdit*>(f->m_entityModel->widget());
    QCOMPARE(dt->date(), QDate());
    dt->setDate(QDate::fromString(testData.value("date").toString(), "yyyy-MM-dd"));
    QCOMPARE(dt->date().toString("yyyy-MM-dd"), testData.value("date").toString());

    cut->setOwnerId(testData.value("repair").toInt());
    QCOMPARE(f->cache.contains(SFieldValueModel::C_repairId), 1);
    QCOMPARE(f->i_logRecord->cache.contains(SLogRecordModel::C_repair), 1);

    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");

    int commited = 0;
    foreach(auto f, cut->m_valueModels)
    {
        if(!f->cache.contains(SFieldValueModel::C_value))
            continue;

        if(f->cache[SFieldValueModel::C_value]->state() == SSingleRowModelBase::ModifiedField::Executed)
            commited++;
    }
    QCOMPARE(commited, 4);
}

void TClassTest::test_fieldsForNewRepair_commitAfterThrow()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto query = std::make_shared<QSqlQuery>(db);
    std::shared_ptr<SFieldValueModel> f = nullptr;

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    insertFieldsForTest(query);


    cut->initSqlQuery(db);
    cut->init(testData.value("device").toInt());

    QCOMPARE(cut->m_valueModels.count(), 5);

    // Простое текстовое поле
    f = cut->m_valueModels.at(0);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "QLineEdit");
    QCOMPARE(f->value(), QString());
    QTest::keyClicks(f->m_entityModel->widget(), testData.value("mac_new").toString());

    cut->setOwnerId(testData.value("repair").toInt());
    QCOMPARE(f->cache.contains(SFieldValueModel::C_repairId), 1);
    QCOMPARE(f->i_logRecord->cache.contains(SLogRecordModel::C_repair), 1);

    QVERIFY(cut->commit() == 1);
    cut->setAllFailed();    // предположим в большой сессии где-то на другом этапе произошел сбой; в throwHandler(int) вызвали этот метод; затем повторная попытка
    query->exec("# >>>>>>>>>>>>>>>> типа сбой <<<<<<<<<<<<<<<<<");
    QVERIFY(cut->commit() == 1);
    query->exec("ROLLBACK;");
}

void TClassTest::test_demo()
{
//    QSKIP("");
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);

    cut->initDemo();

    QCOMPARE(cut->m_valueModels.count(), 4);

    int i = 1;
    foreach(auto f, cut->m_valueModels)
    {
        QVERIFY(f->m_entityModel == nullptr);
        QCOMPARE(f->name(), QString("<extra field %1>").arg(i));
        QCOMPARE(f->value(), QString("<value %1>").arg(i));
        i++;
    }
}

void TClassTest::test_loadRepairFields()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::main());
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto query = std::make_shared<QSqlQuery>(db);
    std::shared_ptr<SFieldValueModel> f = nullptr;

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    insertFieldsForTest(query);
    cut->initSqlQuery(db);
    cut->load(testData.value("repair").toInt());
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_valueModels.count(), 3);

    f = cut->m_valueModels.at(0);
    QVERIFY(f->m_entityModel == nullptr);
    QCOMPARE(f->value(), testData.value("mac_old").toString());

    f = cut->m_valueModels.at(1);
    QVERIFY(f->m_entityModel == nullptr);
    QCOMPARE(f->value(), testData.value("imei").toString());

    f = cut->m_valueModels.at(2);
    QVERIFY(f->m_entityModel == nullptr);
    QCOMPARE(f->value(), testData.value("state").toString());
}

void TClassTest::test_loadFieldModel()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    std::unique_ptr<SFieldModel> cut = nullptr;
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    std::shared_ptr<SFieldValueModel> f = nullptr;

    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    insertFieldsForTest(query);
    query->exec(QString("SELECT `id` FROM `fields` WHERE `_f` = 0 AND FIND_IN_SET(%1,`devices`) AND `archive` = 0 ORDER BY `id`;").arg(testData.value("device").toInt()));
    QVERIFY(query->first() == 1);
//    do
//    {
        cut = std::make_unique<SFieldModel>(query->value(0).toInt(), db);
        QVERIFY(cut.get() != nullptr);
        QCOMPARE(cut->isModelLoaded(), 1);
        QCOMPARE(cut->fieldsForSelectQuery(), "  `id`,\n  `_f`,\n  `name`,\n  `type`,\n  REPLACE(`def_values`,'\\r','') AS 'def_values',\n  `required`,\n  `printable`,\n  `updated_at`,\n  `devices`,\n  `categories`,\n  `archive`,\n  `dev_match`,\n  `input_mask`");
        QCOMPARE(cut->name(), "MAC адрес сетевой карты");
//    } while (query->next());

    query->exec("ROLLBACK;");
}

void TClassTest::test_editField()
{
//    QSKIP("");

    auto db = QSqlDatabase::database(TdConn::session());
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto query = std::make_shared<QSqlQuery>(db);
    std::shared_ptr<SFieldValueModel> f = nullptr;

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    insertFieldsForTest(query);
    cut->initSqlQuery(db);
    cut->load(testData.value("repair").toInt());
    QCOMPARE(cut->m_valueModels.count(), 3);


    f = cut->m_valueModels.at(0);
    QVERIFY(f->fieldId() != 0);
    QCOMPARE(f->name(), "MAC адрес сетевой карты");
    QVERIFY(f->m_entityModel == nullptr);

    cut->enableEdit(testData.value("device").toInt());
    QCOMPARE(f->cacheSize(), 0);

    QCOMPARE(cut->m_valueModels.count(), 5);
    QVERIFY(f->m_entityModel.get() != nullptr);
    QVERIFY(f->m_entityModel->widget() != nullptr);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "QLineEdit");
    QLineEdit *le = static_cast<QLineEdit*>(f->m_entityModel->widget());
    QCOMPARE(f->m_entityModel->lineEdit()->text(), testData.value("mac_old").toString());
    le->setText(testData.value("mac_new").toString()); // значение поля класса SFieldValueModel изменится только при выполнении commit()
    QCOMPARE(f->commitedData(SFieldValueModel::C_value), testData.value("mac_old").toString());
    f->set_value("00-00-00-00-00-00"); // это значение заменится значением из виджета во время коммита
    QCOMPARE(f->repairId(), testData.value("repair").toInt());

    f = cut->m_valueModels.at(1);
    QVERIFY(f->m_entityModel.get() != nullptr);
    QVERIFY(f->m_entityModel->widget() != nullptr);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "QLineEdit");
    le = static_cast<QLineEdit*>(f->m_entityModel->widget());
    QCOMPARE(le->text(), testData.value("imei").toString());
    le->setText("");
    QVERIFY(f->acquireWidgetValue() == 0);
    QCOMPARE(f->repairId(), testData.value("repair").toInt());
    QCOMPARE(f->cacheSize(), 0);

    cut->commit();
    QCOMPARE(cut->m_valueModels.count(), 5);
    QCOMPARE(cut->m_removeList.count(), 0);


    f = cut->m_valueModels.at(0);  // отредактированное поле (MAC-адрес)
    QCOMPARE(f->i_logTexts.count(), 0);
    QCOMPARE(f->i_logRecord->text(),
             tr("Значение параметра %1 изменено с \"%2\" на \"%3\"")
                .arg(f->name())
                .arg(testData.value("mac_old").toString())
                .arg(testData.value("mac_new").toString())
             );
    QCOMPARE(f->i_logRecord->repair(), testData.value("repair").toInt());

    f = cut->m_valueModels.at(1);  // удалённое поле (IMEI)
    QVERIFY(f->isPrimaryKeyValid() == 0);
    QCOMPARE(f->i_logRecord->cache[SLogRecordModel::C_id]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(f->i_logRecord->text(), tr("Параметр %1 (\"%2\") удалён")
                                             .arg(f->name())
                                             .arg(f->commitedData(SFieldValueModel::C_value)->toString()));
    QCOMPARE(f->i_logRecord->repair(), testData.value("repair").toInt());

    f = cut->m_valueModels.at(3);  // поле для ввода даты, инициализированное при включении режима редактирования. Остаётся пустыы, в БД ничего не пишется.
    QVERIFY(f->m_entityModel.get() != nullptr);
    QVERIFY(f->m_entityModel->widget() != nullptr);
    QCOMPARE(f->m_entityModel->widget()->metaObject()->className(), "SDateEdit");
    SDateEdit *de = static_cast<SDateEdit*>(f->m_entityModel->widget());
    QCOMPARE(de->text(), "--.--.----");

    cut->commit();  // при втором вызове не должно быть запросов к БД

    query->exec("ROLLBACK;");
}

void TClassTest::test_editField_commitAfterThrow()
{
    //    QSKIP("");

        auto db = QSqlDatabase::database(TdConn::session());
        auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
        auto query = std::make_shared<QSqlQuery>(db);
        std::shared_ptr<SFieldValueModel> f = nullptr;

        debugStuff->startSqlLog(db, __func__);
        query->exec(QString("# =========================================== %1").arg(__func__));
        query->exec("BEGIN;");
        insertFieldsForTest(query);
        cut->initSqlQuery(db);
        cut->load(testData.value("repair").toInt());
        QCOMPARE(cut->m_valueModels.count(), 3);


        f = cut->m_valueModels.at(0);
        cut->enableEdit(testData.value("device").toInt());
        QVERIFY(f->m_entityModel->widget() != nullptr);
        QLineEdit *le = static_cast<QLineEdit*>(f->m_entityModel->widget());
        le->setText(testData.value("mac_new").toString()); // значение поля класса SFieldValueModel изменится только при выполнении commit()

        cut->commit();
        cut->setAllFailed();    // предположим в большой сессии где-то на другом этапе произошел сбой; в throwHandler(int) вызвали этот метод; затем повторная попытка
        query->exec("# >>>>>>>>>>>>>>>> типа сбой <<<<<<<<<<<<<<<<<");
        cut->commit();
        query->exec("ROLLBACK;");
}

void TClassTest::test_secondAttemptToEnableEdit()
{
//    QSKIP("");
    auto db = QSqlDatabase::database(TdConn::session());
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto query = std::make_shared<QSqlQuery>(db);
    std::shared_ptr<SFieldValueModel> f = nullptr;

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    insertFieldsForTest(query);
    cut->initSqlQuery(db);
    cut->load(testData.value("repair").toInt());
    QCOMPARE(cut->m_valueModels.count(), 3);

    cut->enableEdit(testData.value("device").toInt());
    QCOMPARE(cut->m_valueModels.count(), 5);
    cut->enableEdit(testData.value("device").toInt());
    QCOMPARE(cut->m_valueModels.count(), 5);

    query->exec("ROLLBACK;");
}

void TClassTest::test_clear()
{
//    QSKIP("");

    auto db = QSqlDatabase::database(TdConn::main());
    auto cut = std::make_unique<SFieldsModel>(std::make_unique<SRepairField>());
    auto query = std::make_shared<QSqlQuery>(db);

    query->exec("BEGIN;");
    insertFieldsForTest(query);
    cut->initSqlQuery(db);
    cut->init(testData.value("device").toInt());
    query->exec("ROLLBACK;");

    QCOMPARE(cut->m_valueModels.count(), 5);
    cut->clear();
    QCOMPARE(cut->m_valueModels.count(), 0);
}


QTEST_MAIN(TClassTest)

#include "tclasstest.moc"
