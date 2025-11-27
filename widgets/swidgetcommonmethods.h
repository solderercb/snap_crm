#ifndef SWIDGETCOMMONMETHODS_H
#define SWIDGETCOMMONMETHODS_H

#include <QString>
#ifdef QT_DEBUG
    #include <SQueryLog>
#endif

class SWidgetCommonMethods
{
public:
    SWidgetCommonMethods();
    ~SWidgetCommonMethods();
    bool submit();
protected:
    virtual int checkInput();
    virtual void beginCommit(){};
    virtual QString queryLogFile(){return "SWidgetCommonMethods";};
    virtual int commitStages(){return 1;};
    virtual bool skip(const int);
    virtual void beginCommit(const int){};
    virtual void commit(const int){};
    virtual void endCommit(const int){};
    virtual void throwHandler(int){};
    virtual void endCommit(){};
    virtual bool manualSubmit();
#ifdef QT_DEBUG
    SQueryLog *i_queryLog;
#define QUERY_LOG_START(className)  i_queryLog->start((className));
#define QUERY_LOG_STOP              i_queryLog->stop();
// Шаблоны для включения/выключения журналирвоания запросов главного соединения (запросы на получение данных, напрример, SELECT):
#define MAINCONN_QUERY_LOG_START(className)     SQueryLog *log = SQueryLog::start(QSqlDatabase::database("connMain"), (className));
#define MAINCONN_QUERY_LOG_STOP                 delete log;
#else
#define QUERY_LOG_START(className)
#define QUERY_LOG_STOP
#define MAINCONN_QUERY_LOG_START(className)
#define MAINCONN_QUERY_LOG_STOP
#endif
private:
    void submitCoreRoutine(const bool standAlone = 0);
    bool submitCoreRoutineWithExceptionHandler();
    bool submitSubroutine(const bool standAlone = 0);
};

#endif // SWIDGETCOMMONMETHODS_H
