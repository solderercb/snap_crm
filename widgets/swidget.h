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
#ifdef QT_DEBUG
    SQueryLog *i_queryLog;
#define QUERY_LOG_START(className)  i_queryLog->start((className));
#define QUERY_LOG_STOP              i_queryLog->stop();
#else
#define QUERY_LOG_START(className)
#define QUERY_LOG_STOP
#endif
signals:

};

#endif // SWIDGET_H
