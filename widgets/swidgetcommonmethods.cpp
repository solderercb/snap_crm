#include "swidgetcommonmethods.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SSingleRowModel>
#ifdef QT_DEBUG
#include <SQueryLog>
#endif

SWidgetCommonMethods::SWidgetCommonMethods()
{
#ifdef QT_DEBUG
    i_queryLog = new SQueryLog();
#endif
}

SWidgetCommonMethods::~SWidgetCommonMethods()
{
#ifdef QT_DEBUG
    delete i_queryLog;
#endif
}

/*  Проверка введённых пользователем данных
 *  Пустышка. Функция должна быть переопределена в наследующем классе.
 *  Возвращает число больше 0 в случае ошибки
*/
int SWidgetCommonMethods::checkInput()
{
    return 0;
}

/* В некоторых случаях этап должен быть пропущен
 * Для этого переопределённый метод должен возвращать 1
*/
bool SWidgetCommonMethods::skip(const int)
{
    return 0;
}

/* Основная процедура записи данных в БД.
 * Параметр standAlone = 1 указывает на необходимость выполнения запросов
 * открытия и закрытия сессии
 */
void SWidgetCommonMethods::submitCoreRoutine(const bool standAlone)
{
    std::unique_ptr<QSqlQuery> query = nullptr;

    SSingleRowModel::checkSystemTime();

    if(standAlone)
        query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));

    for(int stage = 0; stage < commitStages(); ++stage)
    {
        if(skip(stage))
            continue;

        beginCommit(stage);    // операции, которые нужно выполнить до начала сессии

        if(standAlone){
            QUERY_EXEC_TH(query, 1, QUERY_BEGIN);
        }

        commit(stage);
//        throw(Global::ThrowType::Debug);

        if(standAlone){
            QUERY_EXEC_TH(query, 1, QUERY_COMMIT);
        }

        endCommit(stage);    // операции, которые нужно выполнить после закрытия сессии
    }
}

/* Оболочка для основной процедуры записи данных в БД.
 * Предназначен для обработки ошибок (при выполнении запросов) в этом
 * классе.
 */
bool SWidgetCommonMethods::submitCoreRoutineWithExceptionHandler()
{
    try
    {
        submitCoreRoutine(true);
    }
    catch (Global::ThrowType type)
    {
        if (type != Global::ThrowType::ConnLost)
        {
            auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database("connThird"));
            QUERY_COMMIT_ROLLBACK(query, 0);
        }

        throwHandler(type);    // действия в случае возникновения ошибок; например, откат изменений
        return 0;
    }

    return 1;
}

/* Подпрограмма записи данных в БД.
 * Параметр standAlone = 1 указывает, что обработка ошибок выполняется в
 * этом объекте, а так же, что будут выполнены запросы открытия и
 * закрытия сессии
 */
bool SWidgetCommonMethods::submitSubroutine(const bool standAlone)
{
    bool nErr = 1;

    beginCommit();  // операции, выполняемые в самом начале, например, иницилазиция модели данными, введёнными пользователем

#ifdef QT_DEBUG
    if(standAlone){
        i_queryLog->start(queryLogFile());}
#endif

    if(standAlone)
        nErr = submitCoreRoutineWithExceptionHandler();
    else
        submitCoreRoutine();

#ifdef QT_DEBUG
    if(standAlone){
        i_queryLog->stop();}
#endif

    if(nErr)
        endCommit();    // операции, выполняемые в самом конце и если не возникло ошибок

    return nErr;
}

/* Слот, подключаемый к сигналу элементарного виджета (например,
 * QPushButton) в наследнике SWidget. Вызов checkInput() для проверки
 * введённых пользователем данных.
 * Например, виджет SDialogIssueRepair может быть диалоговым окном и
 * перед выполнением запросов необходимо проверить, установил ли
 * пользователь галочку, подтверждающую получение денег.
 */
bool SWidgetCommonMethods::manualSubmit()
{
    if(checkInput())
        return 0;

    return submitSubroutine(true);
}

/* Аналогично manualSubmit(), но должен вызываться из другого составного
 * виджета. Этот метод не отправляет запросы начала и завершения сессии,
 * а также в нём не предусмотрена обработка ошибок.
 * Пример использования: виджет SDialogIssueRepair при создании быстрого
 * ремонта не имет графического интерфейса; проверка правильности ввода
 * данных, открытие и закрытие сессии, а также обработка ошибок
 * производятся в методах класса tabRepairNew.
 */
bool SWidgetCommonMethods::submit()
{
    return submitSubroutine();
}
