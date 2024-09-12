#ifndef SWIDGET_H
#define SWIDGET_H

#include <QObject>
#include <QWidget>
#ifdef QT_DEBUG
#include "squerylog.h"
#endif

class SWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SWidget(QWidget *parent = nullptr, Qt::WindowFlags = Qt::WindowFlags());
    ~SWidget();
    QFont guiFont();
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
protected:
    SWidget* findParentTab();
protected slots:
    virtual void guiFontChanged(){};
};

#endif // SWIDGET_H
