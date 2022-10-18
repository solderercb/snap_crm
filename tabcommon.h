#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <qglobal.h>
#include <QWidget>
#include <QString>
#include "mainwindow.h"
#ifdef QT_DEBUG
#include <QTimer>
#include <QRandomGenerator>
#include "querylog.h"
#endif

class tabCommon : public QWidget
{
    Q_OBJECT

signals:
    void updateTabTitle(QWidget*);
    void activateCaller(QWidget *);
public:
    explicit tabCommon(MainWindow *p = nullptr);
    ~tabCommon();
    virtual bool tabCloseRequest();
    void setCallerPtr(QWidget *ptr){callerPtr = ptr;};
    virtual QString tabTitle() = 0;
    virtual QIcon* tabIcon();
protected:
    QWidget *callerPtr = nullptr;
    QString i_tabTitle = "Untitled";
    QIcon *i_tabIcon = nullptr;
#ifdef QT_DEBUG
    virtual void randomFill() = 0;
    QTimer *test_scheduler, *test_scheduler2, *main_window_test_scheduler, *main_window_test_scheduler2;
    uint test_scheduler_counter = 0;
#endif
protected slots:
#ifdef QT_DEBUG
    virtual void test_scheduler_handler() = 0;
    virtual void test_scheduler2_handler() = 0;
#endif
};


#endif // TABCOMMON_H
