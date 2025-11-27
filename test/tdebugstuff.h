#ifndef TDEBUGSTUFF_H
#define TDEBUGSTUFF_H
#include <memory>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QFile>
#include <QDebug>
#include "../squerylog.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)   // препроцессор заключит конструкцию x в кавычки
#define DBG_VAR_OUT(var)   STR(| var:) << (var)
#define DBG_FUNC_NAME QString::fromStdString(std::string(__func__) + "()")

class TDebugStuff
{
public:
    TDebugStuff();
    std::unique_ptr<QSettings> debugOptions;
    QMap<QString, QVariant> debugLoginOptions;
    QSqlDatabase connMain;
    QSqlDatabase connNtfy;
    QSqlDatabase connThird;
    void readSettings();
    void connect();
    static void setSettingsPath(const QString &settingsPath);
    bool testRecordExists(const QString &group, QMap<QString, QVariant> &testData);
    bool readTestData(const QString &section, QMap<QString, QVariant> &testData);
    void startSqlLog(const QSqlDatabase &database, const QString &className);
private:
    static QString m_settingsPath;
    std::shared_ptr<QSqlQuery> query = nullptr;
    std::unique_ptr<SQueryLog> queryLog = nullptr;
};

#endif // TDEBUGSTUFF_H
