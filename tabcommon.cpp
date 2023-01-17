#include "tabcommon.h"

tabCommon::tabCommon(MainWindow *p) :
    SWidget(p)
{
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
