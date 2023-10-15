#include "tabcommon.h"
#include "global.h"

tabCommon::tabCommon(MainWindow *p) :
    SWidget(p)
{
    installEventFilter(this);
    connect(this, &tabCommon::tabClosed, p, &MainWindow::switchToLastUsedTab);
#ifdef QT_DEBUG
    test_scheduler = new QTimer();
    test_scheduler->setSingleShot(true);
    test_scheduler2 = new QTimer();
    test_scheduler2->setSingleShot(true);
    QObject::connect(test_scheduler, SIGNAL(timeout()), this, SLOT(test_scheduler_handler()));
    QObject::connect(test_scheduler2, SIGNAL(timeout()), this, SLOT(test_scheduler2_handler()));
#endif
}

tabCommon::~tabCommon()
{
    userDbData->updateActivityTimestamp();
    while(tabList.removeOne(this));
    // если вкладка создана с другой вкладки (выбор клиента/ремонта/др.), то сработает механизм переключения на вызвавшую вкладку,
    // иначе переключение на последнюю использованную вкладку
    if(callerPtr == nullptr)
        emit tabClosed();

#ifdef QT_DEBUG
    delete test_scheduler;
    delete test_scheduler2;
#endif
}

bool tabCommon::tabCloseRequest()
{
    return 1;
}

QIcon *tabCommon::tabIcon()
{
    return i_tabIcon;
}

bool tabCommon::eventFilter(QObject*, QEvent *event)
{
//    if(event->type() != QEvent::Paint)
//        qDebug().nospace() << "[" << this << "] tabCommon::eventFilter() | event->type(): " << event->type();

    if(event->type() == QEvent::HideToParent)
        tabList.append(this);

    return false;
}

void tabCommon::setCursorPositionsToZero()
{
    QLineEdit *le;
    QList<QLineEdit*> list = this->findChildren<QLineEdit*>(QRegularExpression(".*"));
    foreach(le, list)
        le->setCursorPosition(0);
}
