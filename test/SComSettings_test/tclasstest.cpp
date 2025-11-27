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
#ifdef PREPROCESSING_EVALUATION_BUILD
#include "scomsettings_processed.h"
#else
#include <SComSettings>
#endif // PREPROCESSING_EVALUATION_BUILD

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
private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newModel();
    void test_load();
    void test_loadAndInitWidgets();
    void test_isReadyForSave();
    void test_save();
    void test_updateWidgetsAndSave();
    void test_loadUpdateSave();
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

void TClassTest::initAuxiliaryModels()
{
    QVector<QString> currencyList = {QObject::tr("Российский рубль"), QObject::tr("Доллар США"), QObject::tr("Украинская гривна"),
                                     QObject::tr("Белорусский рубль"), QObject::tr("Новый израильский шекель"), QObject::tr("Молдавкий лей"),
                                     QObject::tr("Польский злотый"), QObject::tr("Китайский юань"), QObject::tr("Болгарский лев"),
                                     QObject::tr("Узбекский сум"), QObject::tr("Казахста́нский тенге́")};
//    QVector<QString> currencyIdsList = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    QVector<QString> currencyCodeList = {"RUB", "USD", "UAH", "BYN", "ILS", "MDL", "PLN", "CNY", "BGN", "UZS", "KZT"};
    QList<QStandardItem*> *currencySelector;
    for (int i=0; i<currencyList.size(); i++)
    {
        currencySelector = new QList<QStandardItem*>();
        *currencySelector << new QStandardItem(currencyList.at(i)) << new QStandardItem(currencyCodeList.at(i));
        currencyListModel->appendRow(*currencySelector);
    }
    currencyListModel->setObjectName("currency");
    currencyListModel->setHorizontalHeaderLabels({"name", "id"});

    QVector<QString> managersList = {"admin", "boss", "solderer", "snake"};
    QVector<int> managersCodeList = {1,14,33,11};
    QList<QStandardItem*> *managerSelector;
    for (int i=0; i<managersList.size(); i++)
    {
        managerSelector = new QList<QStandardItem*>();
        auto login = new QStandardItem(managersList.at(i));
        auto id = new QStandardItem();
        id->setData(managersCodeList.at(i), Qt::DisplayRole);
        *managerSelector << login << id;
        engineersModel->appendRow(*managerSelector);
    }
    engineersModel->setObjectName("managers");
    engineersModel->setHorizontalHeaderLabels({"name", "id"});

    QVector<QString> RDStatesList = {"Приём в ремонт", "Статус 2", "Статус 3"};
    QVector<int> RDStatesCodeList = {0,40,50};
    QList<QStandardItem*> *RDStateSelector;
    for (int i=0; i<RDStatesList.size(); i++)
    {
        RDStateSelector = new QList<QStandardItem*>();
        auto state = new QStandardItem(RDStatesList.at(i));
        auto id = new QStandardItem();
        id->setData(RDStatesCodeList.at(i), Qt::DisplayRole);
        *RDStateSelector << state << id;
        repairDispatcherStatesModel->appendRow(*RDStateSelector);
    }
    repairDispatcherStatesModel->setObjectName("repairDispatcherStatesModel");
    repairDispatcherStatesModel->setHorizontalHeaderLabels({"name", "id"});

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
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("SComSettings_test", testData);
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
    auto cut = std::make_unique<SComSettings>();
    QCOMPARE(cut->size(), 125);
    QCOMPARE(cut->m_record->count(), 72);
    QCOMPARE(cut->m_record_settings->count(), 34);
    QCOMPARE(cut->m_classDbMap->count(), 106);

    cut->initWidgets();

    QCOMPARE(cut->i_labelWidgets.count(), 119);
    QCOMPARE(cut->i_editorWidgets.count(), 119);
    QCOMPARE(cut->i_editorWidgetsTypes.count(), 119);
    QCOMPARE(cut->i_editorWidgetsDefaults.count(), 23);
    QCOMPARE(cut->i_propertyGroup.count(), 119);

    QCOMPARE(cut->i_propertyGroup.value(SComSettings::C_repairDispatcherDayLimit), 4);

    QLabel *lbl = cut->i_labelWidgets.value(SComSettings::C_repairDispatcherDayLimit, nullptr);
    QVERIFY(lbl != nullptr);
    QCOMPARE(lbl->text(), tr("repairDispatcherDayLimit"));

    QSpinBox *sb;
    sb = dynamic_cast<QSpinBox*>(cut->i_editorWidgets.value(SComSettings::C_repairDispatcherDayLimit));
    QVERIFY(sb != nullptr);
    QCOMPARE(sb->minimum(), 0);
    QCOMPARE(sb->maximum(), 7);

    sb = dynamic_cast<QSpinBox*>(cut->i_editorWidgets.value(SComSettings::C_emailPort));
    QVERIFY(sb != nullptr);
    QCOMPARE(sb->minimum(), 0);
    QCOMPARE(sb->maximum(), 65535);

    SCheckComboBox *scb = static_cast<SCheckComboBox*>(cut->i_editorWidgets[SComSettings::C_repairDispatcherUsers]);
    QVERIFY(scb != nullptr);
    QVERIFY(scb->model() != nullptr);
    QCOMPARE(scb->checked(), QList<int>{});
}

void TClassTest::test_load()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->load();
    QCOMPARE(cut->rec()->count(), 72);
    QCOMPARE(cut->i_query->size(), 35); // все строки таблицы `settings`
    QCOMPARE(cut->loadedValue(cut->dbFieldIndex(SComSettings::C_repairDispatcherUsersJson)).value<t_repairDispatcherUsers>().Users,
             testData.value("repairDispatcherUsers").toStringList());
    QCOMPARE(cut->repairDispatcherUsersJsonVariantCopy().Users.count(), testData.value("repairDispatcherUsers").toStringList().count());
    QCOMPARE(cut->repairDispatcherUsers(), testData.value("repairDispatcherUsers").toStringList().join(','));

    QCOMPARE(cut->repairDispatcherDayLimit(), testData.value("repairDispatcherDayLimit").toInt());
    QCOMPARE(cut->repairDispatcherStatuses(), testData.value("repairDispatcherStatuses").toStringList().join(','));

    QCOMPARE(cut->emailServer(), testData.value("emailServer").toString());
    QCOMPARE(cut->emailPort(), testData.value("emailPort").toInt());
    QCOMPARE(cut->emailTimeout(), testData.value("emailTimeout").toInt());

    query->exec("ROLLBACK;");
}

void TClassTest::test_loadAndInitWidgets()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->load();
    cut->initWidgets();

    auto cb = static_cast<QComboBox*>(cut->i_editorWidgets[SComSettings::C_currencyId]);
    QCOMPARE(cb->currentIndex(), 0);
    QCOMPARE(cb->currentText(), "Российский рубль");

    auto scb = static_cast<SCheckComboBox*>(cut->i_editorWidgets[SComSettings::C_repairDispatcherUsers]);
    QStringList sl = testData.value("repairDispatcherUsers").toStringList();
    foreach (auto v, scb->checked()) { // все отмеченные галочкой пользователи из списка repairDispatcherUsers в debug.ini
        QCOMPARE(sl.contains(QString::number(v)), 1);
    }
    foreach (auto v, scb->unchecked()) { // не отмеченные галочкой пользователи не из списка repairDispatcherUsers в debug.ini
        QCOMPARE(sl.contains(QString::number(v)), 0);
    }

    auto scb2 = static_cast<SCheckComboBox*>(cut->i_editorWidgets[SComSettings::C_repairDispatcherStatuses]);
    QStringList sl2 = testData.value("repairDispatcherStatuses").toStringList();
    foreach (auto v, scb2->checked()) { // все отмеченные галочкой статусы из списка repairDispatcherStatuses в debug.ini
        QCOMPARE(sl2.contains(QString::number(v)), 1);
    }
    foreach (auto v, scb2->unchecked()) { // не отмеченные галочкой статусы не из списка repairDispatcherStatuses в debug.ini
        QCOMPARE(sl2.contains(QString::number(v)), 0);
    }

    auto sb = static_cast<QSpinBox*>(cut->i_editorWidgets[SComSettings::C_repairDispatcherDayLimit]);
    QCOMPARE(sb->value(), testData.value("repairDispatcherDayLimit").toInt());

    QCOMPARE(static_cast<QLineEdit*>(cut->i_editorWidgets[SComSettings::C_emailServer])->text(), testData.value("emailServer").toString());
    QCOMPARE(static_cast<QSpinBox*>(cut->i_editorWidgets[SComSettings::C_emailPort])->value(), testData.value("emailPort").toInt());

    query->exec("ROLLBACK;");
}

void TClassTest::test_isReadyForSave()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();

    cut->set_currencyId(testData.value("currencyId").toString());
    QCOMPARE(cut->cache.count(), 1);

    t_repairDispatcherUsers tmp;
    tmp.fromJsonArray(QString("[1,2,3]").toLocal8Bit());
    cut->set_repairDispatcherUsersJson(tmp);
    QCOMPARE(cut->cache.count(), 3);    // при изменении JSON должны автоматически обновиться поля с отдельными элементами
    cut->set_repairDispatcherDayLimit(5);
    QCOMPARE(cut->repairDispatcherAssignCriteriaJsonVariantCopy().DayLimit, 5);
    QCOMPARE(cut->cache.count(), 5);    // при изменении отдельного поля должен обновиться JSON
}

void TClassTest::test_save()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->set_currencyId(testData.value("currencyId").toString());
    t_repairDispatcherUsers tmp;
//    tmp.Users.append("1");
//    tmp.Users.append("2");
//    tmp.Users.append("3");
    tmp.fromJsonArray(QString("[1,2,3]").toLocal8Bit());
    QCOMPARE(tmp.Users.count(), 3);
//    qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(tmp.Users);
    cut->set_repairDispatcherUsersJson(tmp);
    cut->set_repairDispatcherDayLimit(5);
    cut->SSettingsBase::save();   // запись в таблицу `config`
    QCOMPARE(cut->isDirty(1), 1);    // поле repairDispatcherUsersVariantCopy должно остаться в статусе Updated (табл. `settings`)
    QCOMPARE(cut->cache[SComSettings::C_currencyId]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SComSettings::C_repairDispatcherUsersJson]->state(), SSingleRowModelBase::ModifiedField::Updated);
    cut->saveToTableSettings();
    QCOMPARE(cut->isDirty(1), 0);
//    qDebug().noquote() << "[" << this << "] " << DBG_FUNC_NAME << DBG_VAR_OUT(cut->repairDispatcherUsersVariantCopy().Users);
    QCOMPARE(cut->repairDispatcherUsersJsonVariantCopy().Users.count(), 3);
    query->exec("ROLLBACK;");
}

void TClassTest::test_updateWidgetsAndSave()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);

    // это инициализация полей правильными типами (чтобы обойтись без загрузки данных)
    cut->m_record->setValue(SComSettings::C_id, QVariant(QVariant::Int));    // это поле используется для определения загружены ли дынные из БД
    cut->m_record->setValue(SComSettings::C_currencyId, QVariant(QVariant::String)); // тип данных этого поля используется для форматирования текущ. значения виджета
    cut->set_currencyId("RUB");

    cut->initWidgets();

    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QComboBox *cb = static_cast<QComboBox*>(cut->i_editorWidgets[SComSettings::C_currencyId]);
    QCOMPARE(cb->currentIndex(), 0);
    QCOMPARE(cb->currentText(), "Российский рубль");

    auto scb = static_cast<SCheckComboBox*>(cut->i_editorWidgets[SComSettings::C_repairDispatcherUsers]);
    QCOMPARE(scb->checked(), QList<int>{});   // списко должен быть пустым, т. к. загрузка не производилась
    scb->toggleChecked(1);
    scb->toggleChecked(33);
    static_cast<QLineEdit*>(cut->i_editorWidgets[SComSettings::C_emailServer])->setText(testData.value("emailServer").toString());
    static_cast<QSpinBox*>(cut->i_editorWidgets[SComSettings::C_emailPort])->setValue(testData.value("emailPort").toInt());

    cut->save();
    query->exec("ROLLBACK;");

    QCOMPARE(cut->cache[SComSettings::C_currencyId]->data(), cb->model()->index(0,1).data());
    QCOMPARE(cut->cache[SComSettings::C_currencyId]->state(), SSingleRowModelBase::ModifiedField::Executed);
    QCOMPARE(cut->cache[SComSettings::C_repairDispatcherUsers]->data(), "1,33");
    QCOMPARE(cut->cache[SComSettings::C_emailServer]->data(), testData.value("emailServer"));
    QCOMPARE(cut->cache[SComSettings::C_emailPort]->data(), testData.value("emailPort"));
}

void TClassTest::test_loadUpdateSave()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();

    auto cache_size = [&](){
        int count = 0;
        foreach (auto i, cut->cache)
        {
            if(cut->m_classDbMap->value(cut->cache.key(i), -1) >= 0 && i->state() == SSingleRowModelBase::ModifiedField::Updated)
                count++;
        }
        return count;
    };

    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_shared<QSqlQuery>(db);
    debugStuff->startSqlLog(db, __func__);
    cut->initSqlQuery(db);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    cut->load();
    cut->set_repairDispatcherDayLimit(5);
    cut->repairDispatcherUsersJsonVariantCopy().Users.removeOne(testData.value("repairDispatcherUsers").toStringList().at(0));
    t_repairDispatcherUsers tmp = cut->repairDispatcherUsersJsonVariantCopy();
    tmp.Users.removeOne(testData.value("repairDispatcherUsers").toStringList().at(0));
    cut->set_repairDispatcherUsersJson(tmp);
    QCOMPARE(cache_size(), 2);

    query->exec("ROLLBACK;");
}

void TClassTest::test_report_column_headers()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();

    cut->load();
    QCOMPARE(cut->reportColumnCount(), 119);
    QVERIFY(cut->m_reportFieldsMap->contains("currencyId") == 1);
    QVERIFY(cut->m_reportFieldsMap->contains("isPriceColOpt3Visible") == 1);
}

void TClassTest::test_report_field()
{
//    QSKIP("");
    auto cut = std::make_unique<SComSettings>();

    cut->load();
    QVariant data;
    cut->reportData("currencyId", data);
    QCOMPARE(data, testData.value("currencyId"));
    cut->reportData("isPriceColOpt3Visible", data);
    QCOMPARE(data, QVariant(0));
}

void TClassTest::test_report_additional_fields()
{
    QSKIP("");
    auto cut = std::make_unique<SComSettings>();

    cut->load();
    QVariant data;
}

QTEST_MAIN(TClassTest)

#include "tclasstest.moc"
