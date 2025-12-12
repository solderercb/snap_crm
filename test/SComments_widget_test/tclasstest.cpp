#include <windows.h>
#include <QCoreApplication>
#include <QTextCodec>
#include <QObject>
#include <QtTest>
#include <QStandardItem>
#include "../tdebugstuff.h"
#include "ui_scomments.h"
#include <ProjectGlobals>
#include <memory>
#include <QDate>
#include <QDateTime>
#include <QTimeZone>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlField>
#include <QMenu>
#include <SStandardItemModel>
#include <SLogRecordModel>
#include <SCommentModel>
#include <SCommentsModel>
#include <tabCommon>
#include <SComments>

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
    void test_new_widget();
    void test_add_one_comment();
    void test_add_two_comments();
    void test_add_three_comments();
    void test_edit_comment();
    void test_edit_recent_comment();
    void test_remove_comment();
    void test_context_menu();
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
    debugStuff->readTestData("SComments_widget_test", testData);
}

void TClassTest::cleanupTestCase()
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("ROLLBACK;");
    query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::session()));
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("ROLLBACK;");
}

void TClassTest::test_connection()
{
    QVERIFY(QSqlDatabase::database(TdConn::main()).isOpen() == 1);
//    QVERIFY(SSingleRowModel::checkSystemTime() == 1);
    initAuxiliaryModels();
}

void TClassTest::test_new_widget()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);

    auto db = QSqlDatabase::database(TdConn::main());
    auto query = std::make_unique<QSqlQuery>(db);

    QVERIFY(cut->commentsModel != nullptr);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");
    cut->load(testData.value("repair").toInt());
    query->exec("ROLLBACK;");

    QVERIFY(cut->commentsModel->rowCount() == 3);

}

void TClassTest::test_add_one_comment()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('aaaaa', '2025-11-21 01:52:30', 1, %1, NULL, NULL, NULL);").arg(1));
    cut->load(1);   // загрузка нужна для инициализации модели (настройка кол-ва столбцов)
    QCOMPARE(cut->commentsModel->rowCount(), 1);
    QCOMPARE(cut->commentsModel->columnCount(), 4);

    auto text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    cut->ui->plainTextEdit->setPlainText(text);
//    QTest::keyClick(cut->ui->plainTextEdit, Qt::Key_Return); // имитация нажатия Enter (внимание! выполняетcя запрос COMMIT!)
    cut->addComment();  // прямой вызов метода записи текста из поля для ввода (запись без выполнения запроса COMMIT)

    query->exec("ROLLBACK;");
    QVERIFY(cut->commentsModel->rowCount() == 2);
    QCOMPARE(cut->commentsModel->data(cut->commentsModel->index(0, SCommentModel::Columns::C_text)).toString(), text);
}

void TClassTest::test_add_two_comments()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);
    cut->commentsModel->setObjId(1);
    cut->commentsModel->setColumnCount(4);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    QCOMPARE(cut->commentsModel->columnCount(), 4);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QStringList texts;
    texts << "aaaaa";
    cut->ui->plainTextEdit->setPlainText(texts.value(0));
    cut->addComment();
    QVERIFY(cut->commentsModel->rowCount() == 1);
    QCOMPARE(cut->commentsModel->index(0, SCommentModel::Columns::C_text).data().toString(), texts.value(0));

    texts << "bbbbb";
    cut->ui->plainTextEdit->setPlainText(texts.value(1));
    cut->addComment();

    query->exec("ROLLBACK;");

    QVERIFY(cut->commentsModel->rowCount() == 2);
    QVERIFY(cut->commentsModel->m_modified.count() == 2);

    QCOMPARE(cut->commentsModel->index(1, SCommentModel::Columns::C_text).data().toString(), texts.value(0));
    QCOMPARE(cut->commentsModel->index(0, SCommentModel::Columns::C_text).data().toString(), texts.value(1));
}

void TClassTest::test_add_three_comments()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);
    cut->commentsModel->setObjId(1);
    cut->commentsModel->setColumnCount(4);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    QCOMPARE(cut->commentsModel->columnCount(), 4);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    QStringList texts;
    texts << "aaaaa";
    cut->ui->plainTextEdit->setPlainText(texts.value(0));
    cut->addComment();
    QVERIFY(cut->commentsModel->rowCount() == 1);
    QCOMPARE(cut->commentsModel->index(0, SCommentModel::Columns::C_text).data().toString(), texts.value(0));

    texts << "bbbbb";
    cut->ui->plainTextEdit->setPlainText(texts.value(1));
    cut->addComment();

    texts << "ccccc";
    cut->ui->plainTextEdit->setPlainText(texts.value(2));
    cut->addComment();

    query->exec("ROLLBACK;");

    QVERIFY(cut->commentsModel->rowCount() == 3);
    QVERIFY(cut->commentsModel->m_modified.count() == 3);

    QCOMPARE(cut->commentsModel->index(0, SCommentModel::Columns::C_text).data().toString(), texts.value(2));
    QCOMPARE(cut->commentsModel->index(1, SCommentModel::Columns::C_text).data().toString(), texts.value(1));
    QCOMPARE(cut->commentsModel->index(2, SCommentModel::Columns::C_text).data().toString(), texts.value(0));
}

void TClassTest::test_edit_comment()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('aaaaa', '2025-11-21 01:52:30', 1, %1, NULL, NULL, NULL);").arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('bbbbb', '2025-11-21 01:52:31', 1, %1, NULL, NULL, NULL);").arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('ccccc', '2025-11-21 01:52:32', 1, %1, NULL, NULL, NULL);").arg(1));
    cut->load(1);
    QCOMPARE(cut->commentsModel->rowCount(), 3);

    cut->ui->tableViewComments->selectRow(1);
    cut->edit();
    QCOMPARE(cut->ui->plainTextEdit->property("recordRow").toInt(), 1);
    QCOMPARE(cut->ui->plainTextEdit->toPlainText(), "bbbbb");
    cut->ui->plainTextEdit->setPlainText("bbbbb1");
    cut->updateComment();

    query->exec("ROLLBACK;");

    QCOMPARE(cut->commentsModel->rowCount(), 3);
    QCOMPARE(cut->commentsModel->index(0, SCommentModel::Columns::C_text).data().toString(), "ccccc");
    QCOMPARE(cut->commentsModel->index(1, SCommentModel::Columns::C_text).data().toString(), "bbbbb1");
    QCOMPARE(cut->commentsModel->index(2, SCommentModel::Columns::C_text).data().toString(), "aaaaa");
}

void TClassTest::test_edit_recent_comment()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('aaaaa', '2025-11-21 01:52:30', 1, %1, NULL, NULL, NULL);").arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('bbbbb', '2025-11-21 01:52:31', 1, %1, NULL, NULL, NULL);").arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('ccccc', '2025-11-21 01:52:32', 1, %1, NULL, NULL, NULL);").arg(1));
    cut->load(1);

    query->exec("ROLLBACK;");

    QCOMPARE(cut->commentsModel->rowCount(), 3);

    cut->editLastComment();
    QCOMPARE(cut->ui->plainTextEdit->property("recordRow").toInt(), 0);
    QCOMPARE(cut->ui->plainTextEdit->toPlainText(), "ccccc");
}

void TClassTest::test_remove_comment()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('aaaaa', '2025-11-21 01:52:30', 1, %1, NULL, NULL, NULL);").arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('bbbbb', '2025-11-21 01:52:31', 1, %1, NULL, NULL, NULL);").arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('ccccc', '2025-11-21 01:52:32', 1, %1, NULL, NULL, NULL);").arg(1));
    cut->load(1);
    QCOMPARE(cut->commentsModel->rowCount(), 3);

    cut->commentsModel->remove(1);

    query->exec("ROLLBACK;");

    QCOMPARE(cut->commentsModel->rowCount(), 2);
    QCOMPARE(cut->commentsModel->index(0, SCommentModel::Columns::C_text).data().toString(), "ccccc");
    QCOMPARE(cut->commentsModel->index(1, SCommentModel::Columns::C_text).data().toString(), "aaaaa");

}

void TClassTest::test_context_menu()
{
    auto tab = std::make_unique<tabCommon>();
    auto cut = std::make_unique<SComments>();
    cut->setParentTab(tab.get());
    cut->setMode(SCommentModel::Repair);

    auto db = QSqlDatabase::database(TdConn::session());
    auto query = std::make_unique<QSqlQuery>(db);
    cut->commentsModel->setConnection(db);

    debugStuff->startSqlLog(db, __func__);
    query->exec(QString("# =========================================== %1").arg(__func__));
    query->exec("BEGIN;");

    auto timestamp1 = QDateTime::currentDateTimeUtc().addSecs(-4000).toString("yyyy-MM-dd hh:mm:ss");
    auto timestamp2 = QDateTime::currentDateTimeUtc().addSecs(-300).toString("yyyy-MM-dd hh:mm:ss");
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('aaaaa', '%1', 1, %2, NULL, NULL, NULL);").arg(timestamp1).arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('bbbbb', '%1', 2, %2, NULL, NULL, NULL);").arg(timestamp2).arg(1));
    query->exec(QString("INSERT INTO `comments` (`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`) VALUES ('ccccc', '%1', 1, %2, NULL, NULL, NULL);").arg(timestamp2).arg(1));
    cut->load(1);
    QCOMPARE(cut->commentsModel->rowCount(), 3);

    query->exec("ROLLBACK;");

    // по умолчанию в тесте включено разрешение Редактировать любой комментарий (т. е. нет ограничений по времени или автору)
    QCOMPARE(cut->isEditable(0), 1);
    QCOMPARE(cut->isEditable(1), 1);
    QCOMPARE(cut->isEditable(2), 1);
    auto menuTest = [&](const int row, const int minMenuHeight, const int maxMenuHeight = 90){
        cut->ui->tableViewComments->selectRow(row);
        cut->menuRequest(QPoint(100, 8+row*userDbData->rowHeight()));
        QCOMPARE(cut->tableMenu->isVisible(), 1);
        QVERIFY(cut->tableMenu->height() > minMenuHeight);
        QVERIFY(cut->tableMenu->height() < maxMenuHeight);
        cut->tableMenu->hide();
    };
    menuTest(0, 48);
    menuTest(1, 48);
    menuTest(2, 48);

    bool editAnyCommentBackup = permissions->editAnyComment;
    permissions->editAnyComment = 0;
    QCOMPARE(cut->isEditable(0), 1);    // комментарий ccccc оставлен 5 минут назад
    QCOMPARE(cut->isEditable(1), 0);    // комментарий bbbbb оставлен другим пользователем
    QCOMPARE(cut->isEditable(2), 0);    // комментарий aaaaa оставлен более часа назад

    menuTest(0, 48);        // в меню все три пункта
    menuTest(1, 16, 30);    // в меню только один пункт
    menuTest(2, 16, 30);    // в меню только один пункт

    permissions->editAnyComment = editAnyCommentBackup;

}

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
