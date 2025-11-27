#include <QtTest>
#include "../tdebugstuff.h"
#include <SLocalSettings>
#include <ProjectGlobals>
#include "../../models/ssinglerowmodel.h"

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
    void test_x();
};

TClassTest::TClassTest()
{

}

TClassTest::~TClassTest()
{

}

void TClassTest::insertFieldsForTest(std::shared_ptr<QSqlQuery> query)
{
/*
    query->exec(QUERY_NEW_ID("id", "fields"));
    if(!query->next())
        return;

    int id = 0;
    QVariant val = query->value("id");

    if(val.isNull())
    {
        if(query->value("count").toInt() == 0) // Если таблица не содержит записей
        id = 1;

        return;
    }

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
*/
}

void TClassTest::initTestCase()
{
    initClients4Test();
    TDebugStuff::setSettingsPath(STR(_PWD/debug.ini));
    debugStuff = std::make_unique<TDebugStuff>();
    debugStuff->readTestData("repair_fields", testData);
}

void TClassTest::cleanupTestCase()
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connMain"));
    query->exec("ROLLBACK;");
    query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));
    query->exec("ROLLBACK;");

}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database("connMain").isOpen() == 1);
    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
}

void TClassTest::test_x()
{
    QSKIP("");
}



QTEST_MAIN(TClassTest)

#include "tclasstest.moc"
