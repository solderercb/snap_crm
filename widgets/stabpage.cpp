#include "stabpage.h"
#ifdef QT_DEBUG
#include <QTimer>
#include <QRandomGenerator>
#endif

STabPage::STabPage(QWidget *parent) :
    SWidget(parent),
    m_parentTab(parent)
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

bool STabPage::pageCloseRequest()
{
    return 1;
}

void STabPage::setCursorPositionsToZero()
{
    QLineEdit *le;
    QList<QLineEdit*> list = this->findChildren<QLineEdit*>(QRegularExpression(".*"));
    foreach(le, list)
        le->setCursorPosition(0);
}
