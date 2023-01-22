#include "swidget.h"

SWidget::SWidget(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent, flags)
{
#ifdef QT_DEBUG
    i_queryLog = new SQueryLog();
#endif
}

SWidget::~SWidget()
{
#ifdef QT_DEBUG
    delete i_queryLog;
#endif
}
