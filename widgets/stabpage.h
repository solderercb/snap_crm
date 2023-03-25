#ifndef STABPAGE_H
#define STABPAGE_H

#include <QObject>
#include "widgets/swidget.h"
#ifdef QT_DEBUG
#include <QTimer>
#include <QRandomGenerator>
#endif

class STabPage : public SWidget
{
    Q_OBJECT
public:
    explicit STabPage(QWidget *parent = nullptr);
    virtual bool pageCloseRequest();
protected:
    void setCursorPositionsToZero();
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

#endif // STABPAGE_H
