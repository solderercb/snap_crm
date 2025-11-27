#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "../../models/srepairstatuslog.h"
#include <ProjectGlobals>
#include <SLogRecordModel>

class TClassTest : public QObject
{
    Q_OBJECT

public:
    TClassTest();
    ~TClassTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_connection();
    void test_newEmptyModel();
    void test_modelCommit();
    void test_modelCommit2();
    void test_modelRepeatedCommit();
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
    auto debugStuff = std::make_unique<TDebugStuff>();
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
    auto cut = std::make_unique<SRepairStatusLog>();

    QVERIFY(cut->m_record->count() == 7);
    QVERIFY(cut->cache.size() == 0);
}

void TClassTest::test_modelCommit()
{
    auto cut = std::make_unique<SRepairStatusLog>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);
    query->exec("BEGIN;");

    cut->set_manager(userDbData->id());
    cut->set_engineer(userDbData->id());
    cut->set_repair(userDbData->id());
    cut->set_status(1);
    QCOMPARE(cut->cache.count(), 4);

    cut->commit();
    QCOMPARE(cut->cache.count(), 7);
    int executedFields = 0;
    foreach (auto f, cut->cache) {if(f->state() == SSingleRowModelBase::ModifiedField::Executed) executedFields++;}
    QCOMPARE(executedFields, 1);

    query->exec("ROLLBACK;");
}

/* Проверка методов set_*Index() */
void TClassTest::test_modelCommit2()
{
    auto cut = std::make_unique<SRepairStatusLog>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);

    cut->set_managerIndex(0);
    cut->set_engineerIndex(0);
    cut->set_repair(userDbData->id());
    cut->set_status(1);
    QCOMPARE(cut->cache.count(), 4);
    QCOMPARE(cut->manager(), 1);
    QCOMPARE(cut->engineer(), 1);

    query->exec("BEGIN;");
    cut->commit();
    query->exec("ROLLBACK;");

    QCOMPARE(cut->cache.count(), 7);
    int executedFields = 0;
    foreach (auto f, cut->cache) {if(f->state() == SSingleRowModelBase::ModifiedField::Executed) executedFields++;}
    QCOMPARE(executedFields, 1);

}

// Тест повторного вызова метода commit
// В случае модели SRepairStatusLog вызов без изменения поля C_state должнен заканчиваться сбоем
// Чтобы коммит был выполнен успешно, перед его повторным вызовом нужно указать другой код статуса
void TClassTest::test_modelRepeatedCommit()
{
    auto cut = std::make_unique<SRepairStatusLog>();
    auto db = QSqlDatabase::database("connThird");
    auto query = std::make_unique<QSqlQuery>(db);

    cut->set_managerIndex(0);
    cut->set_engineerIndex(0);
    cut->set_repair(userDbData->id());
    cut->set_status(1);
    QCOMPARE(cut->cache.count(), 4);
    QCOMPARE(cut->manager(), 1);
    QCOMPARE(cut->engineer(), 1);

    query->exec("BEGIN;");
    cut->commit();

    QCOMPARE(cut->cache.count(), 7);

    int executedFields = 0;
    foreach (auto f, cut->cache) {if(f->state() == SSingleRowModelBase::ModifiedField::Executed) executedFields++;}
    QCOMPARE(executedFields, 1);

    QVERIFY(cut->isDirty() == 0);
    cut->set_status(1); // Попытка задать тот же код статуса; в кэше ничего не должно поменяться, т. к. этот код уже записан в БД (Executed)
    QVERIFY(cut->checkObligatoryFields() == 0);
    QVERIFY(cut->commit() == 1); // метод вернёт 1, т. к. не будет причин для выполнения запроса

    cut->set_status(2); // другой код статуса
//    foreach(auto f, cut->cache)
//    {
//        qDebug() << "[" << this << "] () | " << cut->cache.key(f) << ":" << f->state();
//    }
    QVERIFY(cut->checkObligatoryFields() == 1);
    QVERIFY(cut->commit() == 1);
    QCOMPARE(cut->cache[SRepairStatusLog::C_repair]->state(), SSingleRowModelBase::ModifiedField::Updated);
    QCOMPARE(cut->cache[SRepairStatusLog::C_status]->state(), SSingleRowModelBase::ModifiedField::Executed);

    query->exec("ROLLBACK;");

}


QTEST_APPLESS_MAIN(TClassTest)

#include "tclasstest.moc"
