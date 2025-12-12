#include "threadconnections.h"
#include <QThread>

/* Возвращает название основного соединения с добавлением id вызывающего потока */
QString TdConn::main()
{
    return QString("main_td%1").arg((quintptr)QThread::currentThreadId());
}

/* Возвращает название соединения, используемого в транзакционных запросах, с добавлением id вызывающего потока */
QString TdConn::session()
{
    return QString("session_td%1").arg((quintptr)QThread::currentThreadId());
}
