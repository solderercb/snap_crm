#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <qglobal.h>
#include "widgets/swidget.h"
#include <QString>
#include "mainwindow.h"
#ifdef QT_DEBUG
#include <QTimer>
#include <QRandomGenerator>
#endif

class tabCommon : public SWidget
{
    Q_OBJECT

signals:
    void updateTabTitle(QWidget*);
    void activateCaller(QWidget *);
    void tabClosed();
public:
    explicit tabCommon(MainWindow *p = nullptr);
    ~tabCommon();
    virtual bool tabCloseRequest();
    void setCallerPtr(QWidget *ptr){callerPtr = ptr;};
    virtual QString tabTitle(){return "tabCommon";};
    virtual QIcon* tabIcon();
protected:
    QWidget *callerPtr = nullptr;
    QString i_tabTitle = "Untitled";
    QIcon *i_tabIcon = nullptr;
    bool eventFilter(QObject*, QEvent*) override;
    void setCursorPositionsToZero();
#ifdef QT_DEBUG
    virtual void randomFill(){};
    QTimer *test_scheduler, *test_scheduler2, *main_window_test_scheduler, *main_window_test_scheduler2;
    uint test_scheduler_counter = 0;
#endif
protected slots:
#ifdef QT_DEBUG
    virtual void test_scheduler_handler(){};
    virtual void test_scheduler2_handler(){};
#endif
};


#endif // TABCOMMON_H
